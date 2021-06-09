/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:35:08 by dess              #+#    #+#             */
/*   Updated: 2021/06/08 19:26:35 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cstddef>
#include <cstring>
#include <errno.h>
#include <exception>
#include <fcntl.h>
#include <inttypes.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>	
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>

#define MAX_CONN 3

class Socket
{
  public:
	// Constructors
	Socket(void) throw(Socket::SocketException);
	Socket(int fd, bool blocking) throw(Socket::SocketException);
	Socket(const Socket &other) throw(Socket::SocketException);
	Socket &operator=(const Socket &other);
	~Socket(void);

	// Getters
	int Fd(void) const;
	struct sockaddr_in infos(void) const;

	// Member functions
	void listen(const int port) throw(Socket::SocketException);
	void close(void);
	Socket accept(void) throw(Socket::SocketException);
	std::string readContent(void) throw(Socket::SocketException);
	void		serverResponse(void);
	void		Get(void);
	void		Post(void);
	void		Delete(void);
	void		badRequest(void);

	// Operator overloading
	bool operator==(const int fd) const;
	bool operator==(const Socket &other) const;

	// Member exceptions
	class SocketException : public std::exception
	{
	  public:
		const char *what(void) const throw();
	};

  private:
	int								_fd;
	int								_opt;
	struct sockaddr_in				_address;
	socklen_t						_socklen;
	char							_buffer[30];
	std::vector<std::string>		_infos;
};

#endif
