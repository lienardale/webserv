/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 09:31:19 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/10 18:06:50 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "handleRequest.hpp"
#include "response.hpp"
#include "socket.hpp"
#include "statusCode.hpp"
#include "webserv.hpp"
#include <iostream>
#include <sys/epoll.h>
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
        _add_fd_to_poll(_epoll_fd, it->first, EPOLLIN);

    _run = true;
    while (_run)
    {
        event_count = epoll_wait(_epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < event_count; i++)
            _handleReady(_epoll_fd, events[i].data.fd, &events[i]);
    }
}

/*
 *	Fonction qui se charge de la lecture des données reçues par le serveur via la socket client
 *	@Parametres: Le fd sur lequel la lecture doit se faire
 *	@Infos La fonction lève une SocketException si erreur
 */
void http::Server::_handleReady(int epoll_fd, const int fd, struct epoll_event *event) throw(Socket::SocketException)
{
    std::map< int, t_serverData >::iterator found = _serverSet.find(fd);
    Socket sock(fd, true);

    try
    {
        if (found != _serverSet.end())
        {

            int accepted = sock.accept().Fd();
            _add_fd_to_poll(epoll_fd, accepted, EPOLLIN);
            _requests[accepted] = std::make_pair(http::Request(), found->second);
        }
        else if (event->events & EPOLLIN)
        {
            const std::string content = sock.readContent();
            _requests[fd].first.parse(content);
            std::pair< http::Request, t_serverData > data = _requests[fd];

            if (data.first.isFinished() || content.size() == 0)
            {
                http::Response response;
                if ((int)data.first.header("body").size() > data.second.client_max_body_size)
                    response = http::Response(http::PAYLOAD_TOO_LARGE);
                else
                    response = handleRequest(data.first, data.second);
                _log(data.first, response);
                sock.send(response.toString(data.second.error_page));
                sock.close();
                _requests.erase(fd);
            }
        }
    }
    catch (Socket::SocketException &e)
    {
        http::Response response = http::Response(http::INTERNAL_SERVER_ERROR);
        _log(_requests[fd].first, response);
        sock.send(response.toString(_requests[fd].second.error_page));
    }
    catch (ParsingException &e)
    {
        http::Response response = http::Response(http::BAD_REQUEST);
        sock.send(response.toString(_requests[fd].second.error_page));
        _log(_requests[fd].first, response);
        sock.close();
        _requests.erase(fd);
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
        _run = false;
        for (std::map< int, std::pair< http::Request, t_serverData > >::iterator it = _requests.begin();
             it != _requests.end(); it++)
            Socket(it->first, true).close();
        for (std::map< int, t_serverData >::iterator it = _serverSet.begin(); it != _serverSet.end(); it++)
            Socket(it->first, true).close();
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
