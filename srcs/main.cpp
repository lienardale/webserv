/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/15 19:09:50 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include <asm-generic/socket.h>
#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

void quit(SOCKET serverSocket, int exitCode)
{
	close(serverSocket);
	exit(exitCode);
}

int main(void)
{
	struct sockaddr_in address;
	SOCKET sock, new_sock;
	char buffer[600];
	struct timeval tv;

	bool run(true);
	int opt(1);
	socklen_t address_len(sizeof(address));
	tv.tv_sec = 10;
	tv.tv_usec = 500000;

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	/**************************************************************************
	 *					Sockets init
	 *************************************************************************/
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "Error: socket failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		std::cerr << "Error: setsockopt " << strerror(errno) << std::endl;
		quit(sock, EXIT_FAILURE);
	}

	// socket become a non-blocking sockets
	fcntl(sock, F_SETFL, O_NONBLOCK);

	if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Error: binding failed: " << strerror(errno) << std::endl;
		quit(sock, EXIT_FAILURE);
	}

	if (listen(sock, 3) < 0)
	{
		std::cerr << "Error: listen failed: " << strerror(errno) << std::endl;
		quit(sock, EXIT_FAILURE);
	}

	/**************************************************************************
	 *				Sockets set init
	 *************************************************************************/

	fd_set current_sockets, ready_sockets;
	int selectResult;
	FD_ZERO(&current_sockets);
	FD_SET(sock, &current_sockets);
	/**************************************************************************
	 *				Main loop
	 *************************************************************************/
	while (run)
	{
		// Make a copy because select is destructive !!
		ready_sockets = current_sockets;

		if ((selectResult = select(FD_SETSIZE, &ready_sockets, NULL, NULL, &tv)) < 0)
		{
			std::cerr << "Error: select " << strerror(errno) << std::endl;
			quit(sock, EXIT_FAILURE);
		}
		else if (selectResult == 0)
		{
			std::cerr << "Error: timeout" << std::endl;
			break;
		}

		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets))
			{
				if (i == sock)
				{
					if ((new_sock = accept(sock, (struct sockaddr *)&address, &address_len)) < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "Error: accept failed " << strerror(errno) << std::endl;
							run = false;
							break;
						}
					}
					FD_SET(new_sock, &current_sockets);
				}
				else
				{
					recv(i, buffer, sizeof(buffer), 0);
					std::cout << buffer << std::endl << SO_RCVBUF << std::endl;
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	close(sock);
	return 0;
}
