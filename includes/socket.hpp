/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Updated: 2021/06/10 19:04:52 by dboyer           ###   ########.fr       */
/*   Updated: 2021/06/22 09:36:46 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "parsing/dataStructure.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <inttypes.h>
#include <iostream>
#include <iterator>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <wait.h>

#define MAX_CONN 3

class Socket
{
  public:
	// Constructors
	Socket( void ) throw( Socket::SocketException );
	Socket( int fd, bool blocking ) throw( Socket::SocketException );
	Socket( const Socket &other ) throw( Socket::SocketException );
	Socket &operator=( const Socket &other );
	~Socket( void );

	// Getters
	int Fd( void ) const;
	struct sockaddr_in infos( void ) const;

	// Member functions
	void listen( const int port, const std::string ) throw( Socket::SocketException );
	void close( void );
	Socket accept( void ) throw( Socket::SocketException );
	void readContent( void ) throw( Socket::SocketException );
	void serverResponse( t_serverData data );
	void Get( void );
	void Post( void );
	void Delete( void );
	void badRequest( void );
	std::string Cgi( void );
	bool php_file( void );

	// Operator overloading
	bool operator==( const int fd ) const;
	bool operator==( const Socket &other ) const;

	// Member exceptions
	class SocketException : public std::exception
	{
	  public:
		const char *what( void ) const throw();
	};

  private:
	int _fd;
	int _opt;
	struct sockaddr_in _address;
	socklen_t _socklen;
	char _buffer[ 30 ];
	std::vector< std::string > _infos;
	std::string _request;
};

#endif
