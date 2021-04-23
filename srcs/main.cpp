/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/23 10:50:14 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "socket.hpp"
#include "webserv.hpp"
#include <csignal>

http::Server SERVER(8000, 10);

void handleSignal(int sig)
{
	(void)sig;
	SERVER.stop();
}

int main(void)
{
	// Signal handling
	std::signal(SIGINT, handleSignal);
	std::signal(SIGTERM, handleSignal);
	std::signal(SIGQUIT, handleSignal);

	SERVER.listen();

	return 0;
}

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
