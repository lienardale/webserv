/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/15 17:39:16 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	error_syscall(std::string message)
{
	std::cout << message << " .errno: " << errno << std::endl;
	exit(EXIT_FAILURE);
}

void	socket_close(Socket	&sock)
{
	close(sock.fd);
}

void	socket_send(Socket &sock)
{
	std::string response = "HTTP/1.0 200 OK\r\n\r\nHello";
	send(sock.connect, response.c_str(), response.size(), 0);
}

void	socket_read(Socket &sock)
{
	for ( ; ; )
	{
		std::cout << "waiting for a connection on port 4222\n" << std::endl;
		int	addrlen = sizeof(sock.sockaddr);

		if ((sock.connect = accept(sock.fd, (struct sockaddr*)&sock.sockaddr, (socklen_t*)&addrlen)) == -1)
			error_syscall("Failed to grab connection");

		char	buffer[3000];
		if (read(sock.connect, buffer, 3000) == -1)
			error_syscall("Failed to read");
		std::cout << "-- INFOS --\n\n" << buffer << std::endl << std::endl;

		std::string response = "HTTP/1.0 200 OK\r\n\r\n<h1>NGINX<h1>";
		write(sock.connect, response.c_str(), response.size());
		//socket_send(sock);
		close(sock.connect);
	}
}

void	socket_listen(Socket &sock)
{
	sock.sockaddr.sin_family = AF_INET;
	sock.sockaddr.sin_addr.s_addr = INADDR_ANY;
	sock.sockaddr.sin_port = htons(4222);

	if (bind(sock.fd, (struct sockaddr*)&sock.sockaddr, sizeof(sock.sockaddr)) == -1)
		error_syscall("Failed to bind to port 4222");
	if (listen(sock.fd, 10) == -1)
		error_syscall("Failed to listen on socket");
}

void	socket_create(Socket &sock)
{
	if ((sock.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		error_syscall("Failed to create socket");
}

int		main(void)
{
	Socket	sock;

	socket_create(sock);
	socket_listen(sock);
	socket_read(sock);
	//socket_send(sock);
	socket_close(sock);

	return 0;
}
