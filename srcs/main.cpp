/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/15 14:16:43 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	socket_close(Socket	&sock)
{
	close(sock.connect);
	close(sock.fd);
}

void	socket_read(Socket &sock)
{
	int	addrlen = sizeof(sock.sockaddr);

	if ((sock.connect = accept(sock.fd, (struct sockaddr*)&sock.sockaddr, (socklen_t*)&addrlen)) == -1)
	{
		std::cerr << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	char	buffer[100];
	if (read(sock.connect, buffer, 100) == -1)
	{
		std::cerr << "Failed to read. errno: " << errno << std::endl;
		exit (EXIT_FAILURE);
	}
	std::cout << "You just said: " << buffer << std::endl;
}

void	socket_listen(Socket &sock)
{
	sock.sockaddr.sin_family = AF_INET;
	sock.sockaddr.sin_addr.s_addr = INADDR_ANY;
	sock.sockaddr.sin_port = htons(42);

	if (bind(sock.fd, (struct sockaddr*)&sock.sockaddr, sizeof(sock.sockaddr)) == -1)
	{
		std::cerr << "Failed to bind to port 42. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(sock.fd, 10) == -1)
	{
		std::cerr << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	socket_send(Socket &sock)
{
	std::string response = "Good talking to you\n";
	send(sock.connect, response.c_str(), response.size(), 0);
}

void	socket_create(Socket &sock)
{
	if ((sock.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

int		main(void)
{
	Socket	sock;

	socket_create(sock);
	socket_listen(sock);
	socket_read(sock);
	socket_send(sock);
	socket_close(sock);

	return 0;
}
