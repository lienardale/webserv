/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:04:52 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/29 19:29:41 by alienard         ###   ########.fr       */
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
#include <sys/wait.h>
#include <algorithm>
// #include <wait.h>
#include "parsing/dataStructure.hpp"
#include "dirent.h"
#include "cgi.hpp"

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
	std::string get_request( void ) const;
	struct sockaddr_in infos( void ) const;

	// Member functions
	void listen( const int port, const std::string ) throw( Socket::SocketException );
	void close( void );
	Socket accept( void ) throw( Socket::SocketException );
	void readContent( void ) throw( Socket::SocketException );
	void parseRequest( void );
	void serverResponse( t_serverData data );
	void Get( t_serverData data );
	void Post( void );
	void Delete( t_serverData data );
	void badRequest( void );
	void setCgiEnv( void );
	std::string Cgi( void );
	bool php_file( void );
	void directoryListing( std::string file, t_serverData data );
	void sendpage( t_serverData data);
	void headerCode(std::string content, int code, t_serverData data);
	t_locationData* initLocation(t_serverData data);
	bool locAutoindex( t_serverData data );
	bool methodAllowed( t_serverData data );

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
	std::string	_content;
	std::string	_code;
	t_locationData *_loc;
	std::map<std::string, std::string> m_request;
};

#endif
