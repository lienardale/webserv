/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:04:52 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/02 18:31:51 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "parsing/dataStructure.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <inttypes.h>
#include <iostream>
#include <iterator>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
// #include <wait.h>
#include "dirent.h"
#include "parsing/dataStructure.hpp"
#include "request.hpp"

#define MAX_CONN 3

class Socket
{
  public:
    // Constructors
    Socket(void) throw(Socket::SocketException);
    Socket(int fd, bool blocking) throw(Socket::SocketException);
    Socket(const Socket &other) throw(Socket::SocketException);
    Socket &operator=(const Socket &other);
    ~Socket(void);

    // Getters
    int Fd(void) const;
    struct sockaddr_in infos(void) const;
    int port(void) const;

    // Member functions
    void listen(const int port, const std::string) throw(Socket::SocketException);
    void close(void);
    Socket accept(void) throw(Socket::SocketException);
    std::string readContent(void);
    void send(const std::string content) throw(SocketException);

    // Operator overloading
    bool operator==(const int fd) const;
    bool operator==(const Socket &other) const;

    // Member exceptions
    class SocketException : public std::exception
    {
      public:
        const char *what(void) const throw();
    };

  private:
    int _fd;
    int _opt;
    struct sockaddr_in _address;
    socklen_t _socklen;
};

#endif
