/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/19 14:18:27 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void quit(SOCKET serverSocket, int exitCode)
{
	close(serverSocket);
	exit(exitCode);
}

int main(void)
{
	Socket serverSocket;
	serverSocket.listen(8000);

	while (1)
	{
		std::cout << "loop" << std::endl;
		try
		{
			Socket client = serverSocket.accept();
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
	Socket test(8000, true, true);
				else
				{
>>>>>>> main
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
/*void error_syscall(std::string message) {
  std::cout << message << " .errno: " << errno << std::endl;
  exit(EXIT_FAILURE);
}

void socket_close(Socket &sock) { close(sock.fd); }

void socket_send(Socket &sock) {
  std::string content = "<h1>404 Not Found</h1>";
  std::fstream f;

  if (sock.infos.size() >= 3 && sock.infos[0] == "GET") {
	if (sock.infos[1] == "/")
	  f.open("www/index.html");
	else
	  f.open(("www" + sock.infos[1]).c_str(), std::ios::in);
	if (f) {
	  std::string page((std::istreambuf_iterator<char>(f)),
					   std::istreambuf_iterator<char>());
	  content = page;
	}
	f.close();
  }
  std::ostringstream oss;
  oss << "HTTP/1.1 200 OK\r\n\r\n";
  oss << content;
  send(sock.connect, oss.str().c_str(), oss.str().size(), 0);
}

void socket_read(Socket &sock) {
  for (;;) {
	int addrlen = sizeof(sock.sockaddr);
	char buffer[3000];

	std::cout << "waiting for a connection on port 4222\n" << std::endl;
	if ((sock.connect = accept(sock.fd, (struct sockaddr *)&sock.sockaddr,
							   (socklen_t *)&addrlen)) == -1)
	  error_syscall("Failed to grab connection");

	if (read(sock.connect, buffer, 3000) == -1)
	  error_syscall("Failed to read");
	std::cout << "-- INFOS --\n\n" << buffer << std::endl << std::endl;

	std::istringstream iss(buffer);
	sock.infos =
		std::vector<std::string>((std::istream_iterator<std::string>(iss)),
								 std::istream_iterator<std::string>());
	socket_send(sock);
	close(sock.connect);
  }
}

void socket_listen(Socket &sock) {
  sock.sockaddr.sin_family = AF_INET;         // address family IPv4
  sock.sockaddr.sin_addr.s_addr = INADDR_ANY; // address (localhost)
  sock.sockaddr.sin_port = htons(4222);       // port

  if (bind(sock.fd, (struct sockaddr *)&sock.sockaddr, sizeof(sock.sockaddr)) ==
	  -1)
	error_syscall("Failed to bind to port 4222");
  if (listen(sock.fd, 10) == -1)
	error_syscall("Failed to listen on socket");
}

void socket_create(Socket &sock) {
  // initialise a socket (return fd)
  if ((sock.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // IPv4, binaires, TCP
	error_syscall("Failed to create socket");
}

int main(void) {
  Socket sock;

  socket_create(sock);
  socket_listen(sock);
  socket_read(sock);
  socket_close(sock);

  return 0;
}*/
