/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 11:08:27 by dess              #+#    #+#             */
/*   Updated: 2021/04/19 14:24:31 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket.hpp"

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

/******************************************************************************
 *			Constructors
 *****************************************************************************/
Socket::Socket(int fd, bool blocking) throw(Socket::SocketException) : _fd(fd), _opt(1), _socklen(sizeof(_address))
{
	if (getsockname(_fd, (struct sockaddr *)&_address, &_socklen) < 0)
		throw(Socket::SocketException());
	if (!blocking)
		_initBlocking(_fd);
}

Socket::Socket(void) throw(Socket::SocketException)
	: _fd(socket(AF_INET, SOCK_STREAM, 0)), _opt(1), _socklen(sizeof(_address))
{
	if (_fd < 0)
		throw(Socket::SocketException());
}

Socket::Socket(const Socket &other) throw(Socket::SocketException)
	: _fd(dup(other._fd)), _opt(other._opt), _address(other._address), _socklen(other._socklen)
{
}

Socket &Socket::operator=(const Socket &other) throw(Socket::SocketException)
{
	_fd = dup(other._fd);
	_opt = other._opt;
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
int Socket::Fd() const
{
	return _fd;
}

int Socket::Opt() const
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

/******************************************************************************
 *			Member functions
 *****************************************************************************/
void Socket::listen(const int port) throw(Socket::SocketException)
{
	_initAddress(port, &_address);
	_initOptions(_fd, &_opt);
	_initBind(_fd, &_address, _socklen);
	_initBlocking(_fd);

	if (::listen(_fd, MAX_CONN) < 0)
		throw(Socket::SocketException());
}

Socket Socket::accept(void) throw(Socket::SocketException)
{
	int clientSocket = ::accept(_fd, (struct sockaddr *)&_address, &_socklen);
	if (clientSocket < 0)
		throw(Socket::SocketException());
	return Socket(clientSocket, false);
}

std::string Socket::readContent(void) throw(Socket::SocketException)
{
	std::string result;
	int ret = 0;

	while ((ret = recv(_fd, _buffer, sizeof(_buffer), 0) > 0))
		result += result;
	if (ret < 0)
		throw(Socket::SocketException());
	return result;
}

/******************************************************************************
 *			Operator overloading
 *****************************************************************************/
bool Socket::operator==(const int fd) const
{
	return fd == _fd;
}
