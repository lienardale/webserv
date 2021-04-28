/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcariou <pcariou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 13:29:08 by pcariou           #+#    #+#             */
/*   Updated: 2021/04/23 10:26:49 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <vector>

class Socket
{
  public:
	Socket(void){};
	Socket(int fd);
	Socket(const Socket &src);
	~Socket(void){};
	Socket &operator=(const Socket &rhs);
	int getFd(void) const;

	//	private:
	int fd; // fd creation
	sockaddr_in sockaddr;
	int connect;					  // fd connection
	std::vector< std::string > infos; // http request infos
};

#endif
