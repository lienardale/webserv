/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:27:31 by dess              #+#    #+#             */
/*   Updated: 2021/04/19 19:25:15 by dess             ###   ########.fr       */
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
	Server(uint32_t port, struct timeval timeout);
	~Server(void);

	void listen(void);

  private:
	uint32_t _port;
	struct timeval _timeout;
	Socket _serverSocket;
	fd_set _readSet, _writeSet, _exceptSet;

	Server(void);
	void _handleRead(const int fd);
	void _handleWrite(const int fd);
};

} // namespace http
#endif
