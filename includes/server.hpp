/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:27:31 by dess              #+#    #+#             */
/*   Updated: 2021/04/19 14:43:28 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "socket.hpp"
#include <inttypes.h>

class Server
{

  public:
	Server(uint32_t port, uint32_t timeout);

	void listen(void);

  private:
	Socket _serverSocket;
	fd_set _readSet, _writeSet, _exceptSet;

	Server(void);
};

#endif
