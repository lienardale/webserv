/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 11:08:27 by dess              #+#    #+#             */
/*   Updated: 2021/04/16 15:01:48 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket.hpp"
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/******************************************************************************
 *			Static functions
 *****************************************************************************/
static void _initAddress(uint32_t port, struct sockaddr_in *infosPtr)
{
	infosPtr->sin_family = AF_INET;
	infosPtr->sin_port = htons(port);
	infosPtr->sin_addr.s_addr = htonl(INADDR_ANY);
}

static void _initOptions(int fd, int *opt) throw(Socket::SocketException)
{
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)) < 0)
		throw(Socket::SocketException());
}

static void _initBlocking(int fd) throw(Socket::SocketException)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw(Socket::SocketException());
}

static void _initBind(int fd, struct sockaddr_in *address, socklen_t socklen) throw(Socket::SocketException)
{
	if (bind(fd, (struct sockaddr *)address, socklen) < 0)
		throw(Socket::SocketException());
}

static void _initListen(int fd) throw(Socket::SocketException)
{
	if (listen(fd, MAX_CONN) < 0)
		throw(Socket::SocketException());
}

/******************************************************************************
 *			Constructors
 *****************************************************************************/
Socket::Socket(int fd, bool blocking) throw(Socket::SocketException)
	: _fd(fd), _opt(1), _blocking(blocking), _socklen(sizeof(_address))
{
	if (getsockname(_fd, (struct sockaddr *)&_address, &_socklen) < 0)
		throw(Socket::SocketException());
}

Socket::Socket(uint32_t port, bool blocking, bool listen) throw(Socket::SocketException)
	: _fd(socket(AF_INET, SOCK_STREAM, 0)), _opt(1), _blocking(blocking), _socklen(sizeof(_address))
{
	if (_fd < 0)
		throw(Socket::SocketException());
	_initAddress(port, &_address);
	_initOptions(_fd, &_opt);
	_initBind(_fd, &_address, _socklen);
	if (!_blocking)
		_initBlocking(_fd);
	if (listen)
		_initListen(_fd);
}

Socket::Socket(const Socket &other) throw(Socket::SocketException)
	: _fd(dup(other._fd)), _opt(other._opt), _blocking(other._blocking), _address(other._address),
	  _socklen(other._socklen)
{
	if (_fd < 0)
		throw(Socket::SocketException());
}

Socket &Socket::operator=(const Socket &other) throw(Socket::SocketException)
{
	_fd = dup(other._fd);
	_opt = other._fd;
	_blocking = other._blocking;
	_address = other._address;
	_socklen = other._socklen;

	if (_fd < 0)
		throw(Socket::SocketException());
	return *this;
}

Socket::~Socket(void)
{
	close(_fd);
}

/******************************************************************************
 *			Exceptions
 *****************************************************************************/
const char *Socket::SocketException::what() const throw()
{
	return strerror(errno);
}

/******************************************************************************
 *			Getters
 *****************************************************************************/
int Socket::getFd() const
{
	return _fd;
}

int Socket::getOpt() const
{
	return _opt;
}

socklen_t Socket::size() const
{
	return _socklen;
}

struct sockaddr_in Socket::infos() const
{
	return _address;
}

bool Socket::isBlocking() const
{
	return _blocking;
}

/******************************************************************************
 *			Member functions
 *****************************************************************************/
void Socket::listen(void) throw(Socket::SocketException)
{
	_initListen(_fd);
}

Socket Socket::acceptConn(void) throw(Socket::SocketException)
{
	int clientSocket = accept(_fd, (struct sockaddr *)&_address, &_socklen);
	Socket::SocketException except;
	if (clientSocket < 0 && _blocking)
		throw(except);
	return Socket(clientSocket, _blocking);
}

std::string Socket::readContent(void) throw(Socket::SocketException)
{
	if ((read(_fd, _buffer, sizeof(_buffer)) < 0))
		throw(Socket::SocketException());
	return std::string(_buffer);
}
