/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/16 15:21:10 by dess             ###   ########.fr       */
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
	bool run = true;
	Socket serverSocket(8000, true, true);

	while (run)
	{

		std::cout << "loop" << std::endl;
		try
		{
			Socket client = serverSocket.acceptConn();
			try
			{
				std::string content = client.readContent();
				std::cout << content << std::endl;
			}
			catch (Socket::SocketException &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		catch (Socket::SocketException &e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

/*int main(void)
{
	SOCKET serverSocket, clientSocket, maxSd;
	struct sockaddr_in address;
	char buffer[2000];
	struct timeval tv;

	bool run(true);
	int opt(1);
	socklen_t address_len(sizeof(address));

	tv.tv_sec = 10;
	tv.tv_usec = 500000;

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "Error: socket failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		std::cerr << "Error: setsockopt " << strerror(errno) << std::endl;
		quit(serverSocket, EXIT_FAILURE);
	}

	// socket become a non-blocking sockets
	fcntl(serverSocket, F_SETFL, O_NONBLOCK);

	if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Error: binding failed: " << strerror(errno) << std::endl;
		quit(serverSocket, EXIT_FAILURE);
	}

	if (listen(serverSocket, 3) < 0)
	{
		std::cerr << "Error: listen failed: " << strerror(errno) << std::endl;
		quit(serverSocket, EXIT_FAILURE);
	}

	fd_set current_sockets, ready_sockets;
	int selectResult;
	FD_ZERO(&current_sockets);
	FD_SET(serverSocket, &current_sockets);

	while (run)
	{
		// Make a copy because select is destructive !!
		ready_sockets = current_sockets;
		maxSd = serverSocket;

		if ((selectResult = select(FD_SETSIZE + 1, &ready_sockets, NULL, NULL, &tv)) < 0)
		{
			std::cerr << "Error: select " << strerror(errno) << std::endl;
			quit(serverSocket, EXIT_FAILURE);
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
				if (i == serverSocket)
				{
					if ((clientSocket = accept(serverSocket, (struct sockaddr *)&address, &address_len)) < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "Error: accept failed " << strerror(errno) << std::endl;
							run = false;
							break;
						}
					}
					FD_SET(clientSocket, &current_sockets);
				}
				else
				{
					recv(i, buffer, sizeof(buffer), 0);
					std::cout << buffer << std::endl;
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	close(serverSocket);
	return 0;
}*/
