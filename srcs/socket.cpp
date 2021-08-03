/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 11:08:27 by dess              #+#    #+#             */
/*   Updated: 2021/08/02 18:32:43 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include "cgi.hpp"
#include "request.hpp"
#include "webserv.hpp"
#include <strings.h>

/******************************************************************************
 *	Fonctions statiques permettant l'initialisation d'une socket
 *****************************************************************************/

/*
 * Une fonction qui initialise les informations d'une socket
 *
 * @Parametres: Le nombre de ports et un pointeur sur une structure sockaddr_in
 * @Infos: La structure sockaddr_in est dans le package <netinet/in.h>
 */
static void _initAddress(uint32_t port, struct sockaddr_in *infosPtr, const char *addr_ip)
{
    infosPtr->sin_family = AF_INET;
    infosPtr->sin_port = htons(port);
    infosPtr->sin_addr.s_addr = inet_addr(addr_ip);

    if (infosPtr->sin_addr.s_addr == (uint32_t)-1)
        throw Socket::SocketException();
}

/*
 *	Une fonction qui initialise les options via la fonction setsockopt
 *	@Parametres: Le fd de la socket et un pointeur sur l'option (*opt = 1)
 *	@Infos: La fonction lève une SocketException si setsockopt échoue
 *	@Lien:
 *https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt
 */
static void _initOptions(int fd, int *opt) throw(Socket::SocketException)
{
    // to work on macOS -> suppr SO_REUSEADDR
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)) < 0)
        throw(Socket::SocketException());
}

/*
 *	Une fonction qui change un fd bloquant en un fd non bloquant via fcntl
 *	@Parametres Le fd
 *	@Infos: La fonction lève une SocketException si fcntl échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/fcntl.2.html
 */
static void _initBlocking(int fd) throw(Socket::SocketException)
{
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
        throw(Socket::SocketException());
}

/*
 *	Une fonction qui permet de lier un fd de socket à un nom via la fonction
 *bind
 *	@Parametres Fd de la socket, les informations de la socket (structure
 *sockaddr_in) et la taille de cette structure
 *	@Infos: La fonction lève une SocketException si bind échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/bind.2.html
 */
static void _initBind(int fd, struct sockaddr_in *address, socklen_t socklen) throw(Socket::SocketException)
{
    if (bind(fd, (struct sockaddr *)address, socklen) < 0)
        throw(Socket::SocketException());
}

/******************************************************************************
 *				Constructeurs et Destructeurs
 *****************************************************************************/

/*
 *	Construit un objet Socket à partir d'un fd.
 *	@Parametres: Un fd et un booleen. Si le booleen est false le fd sera
 *changé en non-bloquant.
 *	@Info: le contructeur utisie getsockname et lève une SocketException si
 *celui-ci échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/getsockname.2.html
 */

Socket::Socket(int fd, bool blocking) throw(Socket::SocketException) : _fd(fd), _opt(1), _socklen(sizeof(_address))
{
    if (getsockname(_fd, (struct sockaddr *)&_address, &_socklen) < 0)
        throw(Socket::SocketException());
    if (!blocking)
        _initBlocking(_fd);
}

/*
 *	Constructeur par défaut de Socket. Seule une socket non initialisée est
 *crée.
 *	@Lien: http://manpagesfr.free.fr/man/man2/socket.2.html
 */
Socket::Socket(void) throw(Socket::SocketException)
    : _fd(socket(AF_INET, SOCK_STREAM, 0)), _opt(1), _socklen(sizeof(_address))
{
    if (_fd < 0)
        throw(Socket::SocketException());
}

/*
 *	Constructeur par copie (deepcopy) de Socket
 *	@Lien: http://manpagesfr.free.fr/man/man2/dup.2.html
 */
Socket::Socket(const Socket &other) throw(Socket::SocketException)
    : _fd(other._fd), _opt(other._opt), _address(other._address), _socklen(other._socklen)
{
}

Socket::~Socket(void)
{
}

/******************************************************************************
 *				SocketException
 *****************************************************************************/
/*
 *	Le message correspond au message de errno
 */
const char *Socket::SocketException::what() const throw()
{
    return strerror(errno);
}

/******************************************************************************
 *				Getters
 *****************************************************************************/

/*
 *	Retourne le fd de la socket
 */
int Socket::Fd() const
{
    return _fd;
}

/*
 *	Retourne les infos de la socket (structure sockaddr_in)
 */
struct sockaddr_in Socket::infos() const
{
    return _address;
}

int Socket::port(void) const
{
    return ntohs(_address.sin_port);
}

/*****************************************************************************
 *			Fonction membres
 *****************************************************************************/

/*
 *	Mets la socket en écoute sur un port spécifique
 *	@Parametres: Le port
 *	@Infos: Lève une SocketException si erreur
 */
void Socket::listen(const int port, const std::string addr) throw(Socket::SocketException)
{
    _initAddress(port, &_address, addr.c_str());
    _initOptions(_fd, &_opt);
    _initBind(_fd, &_address, _socklen);
    _initBlocking(_fd);

    if (::listen(_fd, MAX_CONN) < 0)
        throw(Socket::SocketException());
}

/*
 *	Accept la connection avec un client et retourne la socket client
 *	@Infos: Le fd de la socket client retournée est non-bloquant.
 *	La fonction lève une SocketException si erreur
 */
Socket Socket::accept(void) throw(Socket::SocketException)
{
    int clientSocket = ::accept(_fd, (struct sockaddr *)&_address, &_socklen);
    if (clientSocket < 0)
        throw(Socket::SocketException());
    return Socket(clientSocket, false);
}

/*
 *	Lit une partie du contenu reçu par la socket
 *	@Infos: La fonction lève une SocketException si erreur
 */
std::string Socket::readContent(void)
{
    int ret = 0;
    char buffer[300];

    bzero(buffer, sizeof(buffer));
    ret = recv(_fd, buffer, sizeof(buffer), MSG_DONTWAIT);

    if (ret <= 0)
        return "";
    return std::string(static_cast< char * >(buffer), ret);
}

/*
 *	Envoie un contenu via la socket
 *	@Infos: La fonction lève une SocketException si erreur
 */
void Socket::send(const std::string content) throw(SocketException)
{
    std::cout << "--RESPONSE--" << content.c_str() << std::endl;
    if (::send(_fd, content.c_str(), content.size(), 0) <= 0)
        throw Socket::SocketException();
}

/*
 *	Ferme le fd à l'aide la de fonction close
 *	@Infos: Doit être appelée avant la fermeture du programme
 */
void Socket::close(void)
{
    ::close(_fd);
}

/******************************************************************************
 *			Operator overloading
 *****************************************************************************/

Socket &Socket::operator=(const Socket &other)
{
    _fd = other._fd;
    _opt = other._opt;
    _address = other._address;
    _socklen = other._socklen;
    return *this;
}

bool Socket::operator==(const int fd) const
{
    return fd == _fd;
}

bool Socket::operator==(const Socket &other) const
{
    return _fd == other._fd;
}

bool operator==(const int fd, const Socket &s)
{
    return fd == s.Fd();
}
