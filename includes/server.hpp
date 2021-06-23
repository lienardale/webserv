/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:27:31 by dess              #+#    #+#             */
/*   Updated: 2021/06/23 15:18:15 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "parsing/dataStructure.hpp"
#include "socket.hpp"
#include <cstdlib>
#include <inttypes.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <utility>

namespace http
{
class Server
{
  public:
	Server( void );
	Server( const Server &other );
	Server &operator=( const Server &other );
	~Server( void );

	//    Member functions
	void listen( void );
	void stop( void );
	void init( const std::list< t_serverData > &configs, uint32_t timeout );

  private:
	struct timeval _timeout;
	std::list< t_serverData > _configs;
	std::map< int, std::pair< Socket, t_serverData > > _serverSet;
	Socket _currentSock;
	t_serverData _currentData;
	bool _run;
	int _epoll_fd;

	void _handleReady( int epoll_fd, const int fd, struct epoll_event *event ) throw( Socket::SocketException );
	void _watchFds( void ) throw( Socket::SocketException );
};
} // namespace http
#endif
