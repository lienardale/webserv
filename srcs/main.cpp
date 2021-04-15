/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/15 15:02:40 by pcariou          ###   ########.fr       */
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
	close(sock.connect);
	close(sock.fd);
}

void	socket_read(Socket &sock)
{
	int	addrlen = sizeof(sock.sockaddr);

	if ((sock.connect = accept(sock.fd, (struct sockaddr*)&sock.sockaddr, (socklen_t*)&addrlen)) == -1)
		error_syscall("Failed to grab connection");

	char	buffer[100];
	if (read(sock.connect, buffer, 100) == -1)
		error_syscall("Failed to read");
	std::cout << "You just said: " << buffer << std::endl;
}

void	socket_listen(Socket &sock)
{
	sock.sockaddr.sin_family = AF_INET;
	sock.sockaddr.sin_addr.s_addr = INADDR_ANY;
	sock.sockaddr.sin_port = htons(42);

	if (bind(sock.fd, (struct sockaddr*)&sock.sockaddr, sizeof(sock.sockaddr)) == -1)
		error_syscall("Failed to bind to port 42");
	if (listen(sock.fd, 10) == -1)
		error_syscall("Failed to listen on socket");
}

void	socket_send(Socket &sock)
{
	std::string response = "Good talking to you\n";
	send(sock.connect, response.c_str(), response.size(), 0);
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
	socket_send(sock);
	socket_close(sock);

	return 0;
}
