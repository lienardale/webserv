/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 14:54:53 by dess              #+#    #+#             */
/*   Updated: 2021/04/19 19:25:57 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "socket.hpp"
#include <cstdlib>
#include <sys/select.h>
#include <unistd.h>

/******************************************************************************
 *				Constructors
 *****************************************************************************/

http::Server::Server(uint32_t port, struct timeval timeout) : _port(port), _timeout(timeout)
{
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
	FD_ZERO(&_exceptSet);
}

http::Server::~Server(void)
{
}

void http::Server::_handleRead(const int fd)
{

	if (_serverSocket == fd)
	{
		Socket clientSocket = _serverSocket.accept();
		FD_SET(clientSocket.Fd(), &_readSet);
	}
	else
	{
		std::string result = Socket(fd, true).readContent();
		std::cout << result;
		FD_CLR(fd, &_readSet);
		FD_SET(fd, &_writeSet);
	}
}

void http::Server::_handleWrite(const int fd)
{
	std::cout << "write fd: " << fd << std::endl;
	FD_CLR(fd, &_writeSet);
}

void http::Server::listen(void)
{
	try
	{
		_serverSocket.listen(_port);
		FD_SET(_serverSocket.Fd(), &_readSet);
	}
	catch (Socket::SocketException &e)
	{
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	/*
	 *			Variables temps
	 */
	fd_set readyReadSet, readyWriteSet;
	int selectResult;

	while (1)
	{
		// Make a copy because select is destructive !!
		readyReadSet = _readSet;
		readyWriteSet = _writeSet;
		if ((selectResult = select(FD_SETSIZE + 1, &readyReadSet, &readyWriteSet, NULL, &_timeout)) < 0)
		{
			std::cerr << "Error: select " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (selectResult == 0)
		{
			std::cerr << "Error: timeout" << std::endl;
			break;
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &readyReadSet))
				_handleRead(i);
			if (FD_ISSET(i, &readyWriteSet))
				_handleWrite(i);
		}
	}
}
