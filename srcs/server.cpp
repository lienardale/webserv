/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard@student.42.fr <alienard>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 09:31:19 by dboyer            #+#    #+#             */
/*   Updated: 2021/09/06 15:07:42 by alienard@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "handleRequest.hpp"
#include "parsing/dataStructure.hpp"
#include "parsing/parsingExceptions.hpp"
#include "request.hpp"
#include "response.hpp"
#include "socket.hpp"
#include "statusCode.hpp"
#include "webserv.hpp"
#include <algorithm>
#include <cstdio>
#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <netinet/in.h>
#include <ostream>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <utility>

#define MAX_EVENTS 10

/****************************************************************************************
 *				Outils
 ***************************************************************************************/
static void _add_fd_to_poll(int epoll_fd, int socket_fd, uint32_t mask) throw(Socket::SocketException)
{
    struct epoll_event event;

    event.events = mask;
    event.data.fd = socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event))
    {
        close(epoll_fd);
        throw Socket::SocketException();
    }
}

/******************************************************************************
 *			Constructeurs/Destructeurs
 *****************************************************************************/

/*
 *	Initialise un serveur qui écoutera sur un port donnée
 *	@Parametres: Le port qui sera écouté et l'interval (en seconde) entre chaque écoute
 *	@Lien: http://manpagesfr.free.fr/man/man2/select.2.html
 */
http::Server::Server() : _run(false)
{
}

/*
 *	Constructeur par copie
 */
http::Server::Server(const Server &other)
    : _timeout(other._timeout), _serverSet(other._serverSet), _run(other._run), _epoll_fd(other._epoll_fd)
{
}

/*
 *	Construction par assignation
 */
http::Server &http::Server::operator=(const Server &other)
{
    _serverSet = other._serverSet;
    _run = other._run;
    _timeout = other._timeout;
    _run = other._run;
    _epoll_fd = other._epoll_fd;
    return *this;
}

http::Server::~Server(void)
{
}

/****************************************************************************************
 *			Fonctions membres
 ***************************************************************************************/

void http::Server::init(const std::list< t_serverData > &configs, uint32_t timeout)
{
    _timeout.tv_sec = timeout;
    _timeout.tv_usec = 0;

    try
    {
        for (std::list< t_serverData >::const_iterator it = configs.begin(); it != configs.end(); it++)
        {
            Socket s;
            s.listen(it->listen, it->addr_ip);
            _serverSet[s.Fd()] = *it;
        }
    }
    catch (Socket::SocketException &e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*
 *	Fonction qui va mettre le serveur en écoute sur le port spécifié dans le constructeur
 *	@Infos: la fonction arrête le programme si une SocketException est levé
 */
void http::Server::listen(void)
{
    struct epoll_event events[MAX_EVENTS];
    int event_count = 0;

    bzero(events, MAX_EVENTS);
    _epoll_fd = epoll_create1(EPOLL_CLOEXEC);

    for (std::map< int, t_serverData >::iterator it = _serverSet.begin(); it != _serverSet.end(); it++)
    {
        Socket sock(it->first, true);
        _add_fd_to_poll(_epoll_fd, it->first, EPOLLIN);
        if (_serverSetByPort.count(sock.port()) == 0)
            _serverSetByPort[sock.port()] = std::list< t_serverData >();
        _serverSetByPort[sock.port()].push_back(it->second);
    }

    _run = true;
    while (_run)
    {
        event_count = epoll_wait(_epoll_fd, events, _serverSetByPort.size(), -1);
        // MAX_EVENT = _serverSetByPort.size() * 2;
        for (int i = 0; i < event_count && _run; i++)
            _handleReady(_epoll_fd, events[i].data.fd, &events[i]);
    }
}

/*
 *           Fonction qui match le host de la requête avec un t_serverData
 *           qui possède un serverName identique
 */
t_serverData http::Server::_getServerData(Socket &sock, std::string host)
{
    std::list< t_serverData > data = _serverSetByPort[sock.port()];
    for (std::list< t_serverData >::iterator it = data.begin(); it != data.end(); it++)
    {
        if (std::binary_search(it->server_name.begin(), it->server_name.end(), host))
            return *it;
    }
    return data.front();
}

/*
 *	Fonction qui se charge de la lecture des données reçues par le serveur
 *  via la socket client
 *	@Parametres: Le fd sur lequel la lecture doit se faire
 *	@Infos La fonction lève une SocketException si erreur
 */
void http::Server::_handleEpollout(Socket &sock, std::pair< http::Request, t_serverData > &data,
                                   struct epoll_event *event, int epoll_fd) throw(Socket::SocketException)
{
    http::Response response;
    t_serverData serverData = _getServerData(sock, data.first.header("host"));

    if (data.first.isBodyTooLarge())
        response = http::Response(http::PAYLOAD_TOO_LARGE);
    else
        response = handleRequest(data.first, serverData);

    _log(data.first, response);
    sock.send(response.toString(data.second.error_page));
    if (!data.first.keepAlive() || data.first.isBodyTooLarge())
        _removeAcceptedFD(sock);
    else
    {
        data.first.clear();
        event->events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sock.Fd(), event);
    }
}

void http::Server::_handleEpollin(Socket &sock, std::pair< http::Request, t_serverData > &data, const int epoll_fd,
                                  struct epoll_event *event) throw(ParsingException)
{
    const std::string content = sock.readContent();

    data.first.parse(content, data.second.client_max_body_size);
    if (data.first.isFinished())
    {
        // std::cout << data.first << std::endl;
        event->events = EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sock.Fd(), event);
    }
}

void http::Server::_handleReady(int epoll_fd, const int fd, struct epoll_event *event) throw(std::exception)
{
    std::map< int, t_serverData >::iterator found = _serverSet.find(fd);
    Socket sock(fd, true);

    try
    {
        if (found != _serverSet.end())
        {
            int accepted = sock.accept().Fd();
            _add_fd_to_poll(epoll_fd, accepted, EPOLLIN | EPOLLRDHUP);
            _requests[accepted] = std::make_pair(http::Request(), found->second);
        }
        else if (event->events & EPOLLRDHUP)
            _removeAcceptedFD(sock);
        else if (event->events & EPOLLOUT)
            _handleEpollout(sock, _requests[fd], event, epoll_fd);
        else if (event->events & EPOLLIN)
            _handleEpollin(sock, _requests[fd], epoll_fd, event);
    }
    catch (ParsingException &e)
    {
        http::Response response = http::Response(http::BAD_REQUEST);
        response.setHeader("Connection", "close");
        _log(_requests[fd].first, response);
        sock.send(response.toString(_requests[fd].second.error_page));
        _removeAcceptedFD(sock);
    }
    catch (Socket::SocketException &e)
    {
        // remove the client here
        std::cerr << e.what() << std::endl;
        _removeAcceptedFD(sock);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/*
 *	Fonction qui stop le serveur et ferme tous les fd ouverts
 */
void http::Server::stop(void)
{
    std::cout << "Gracefully stopping the server..." << std::endl;
    if (_run)
    {
        try
        {
            _run = false;
            for (std::map< int, std::pair< http::Request, t_serverData > >::iterator it = _requests.begin();
                 it != _requests.end(); it++)
                Socket(it->first, true).close();
            for (std::map< int, t_serverData >::iterator it = _serverSet.begin(); it != _serverSet.end(); it++)
                Socket(it->first, true).close();
        }
        catch (Socket::SocketException &e)
        {
            std::cerr << e.what() << std::endl;
        }
        close(_epoll_fd);
    }
}

/*
 *  Fonction qui affiche les logs sur la sortie standard
 */
void http::Server::_log(const http::Request &request, const http::Response &response) const
{
    time_t now = time(0);
    std::cout << ctime(&now) << "\t";
    int code = response.code();

    if (code >= 400)
        std::cout << RED;
    else if (code >= 200 && code < 300)
        std::cout << GREEN;

    std::cout << response.code() << WHITE << "\t|\t";
    std::cout << request.header("host") << "\t|\t";
    std::cout << request.header("method") << "\t|\t";
    std::cout << request.header("path") << std::endl;
}

/*
 *       Fonction qui retire un fd accepté d'epoll et de la map de requête
 */
void http::Server::_removeAcceptedFD(Socket &sock)
{
    try
    {
        _requests.erase(sock.Fd());
        sock.close();
    }
    catch (Socket::SocketException &e)
    {
        std::cerr << e.what() << std::endl;
    }
    // epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock.Fd(), event);
}
