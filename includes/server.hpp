/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:27:31 by dess              #+#    #+#             */
/*   Updated: 2021/04/23 10:30:36 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "socket.hpp"
#include <inttypes.h>
#include <sys/time.h>
#include <vector>

namespace http
{
class Server
{

  public:
	Server(uint32_t port, uint32_t timeout);
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server(void);

	void listen(void);
	void stop(void);

  private:
	uint32_t _port;
	struct timeval _timeout;
	Socket _serverSocket;
	fd_set _readSet, _writeSet, _exceptSet;
	bool _run;

	Server(void);
	void _handleRead(const int fd) throw(Socket::SocketException);
	void _handleWrite(const int fd);
	void _watchFds(void) throw(Socket::SocketException);
};

} // namespace http
#endif
