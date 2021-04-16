/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:35:08 by dess              #+#    #+#             */
/*   Updated: 2021/04/16 13:16:09 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <cstddef>
#include <exception>
#include <fcntl.h>
#include <inttypes.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX_CONN 3

class Socket
{
  public:
	// Constructors
	Socket(int fd, bool blocking) throw(Socket::SocketException);
	Socket(uint32_t port, bool blocking, bool listen) throw(Socket::SocketException);
	Socket(const Socket &other) throw(Socket::SocketException);
	Socket &operator=(const Socket &other) throw(Socket::SocketException);
	~Socket(void);

	// Getters
	int getFd(void) const;
	int getOpt(void) const;
	bool isBlocking(void) const;
	struct sockaddr_in infos(void) const;
	socklen_t size(void) const;

	// Member functions
	void listen(void) throw(Socket::SocketException);
	Socket acceptConn(void) throw(Socket::SocketException);
	std::string readContent(void) throw(Socket::SocketException);

	// Member exceptions
	class SocketException : public std::exception
	{
	  public:
		const char *what(void) const throw();
	};

  private:
	int _fd;
	int _opt;
	bool _blocking;
	struct sockaddr_in _address;
	socklen_t _socklen;
	char _buffer[500];

	Socket(void);
};

#endif
