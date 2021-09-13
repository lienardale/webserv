/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:27:31 by dess              #+#    #+#             */
/*   Updated: 2021/09/08 12:07:01 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "parsing/dataStructure.hpp"
#include "parsing/parsingExceptions.hpp"
#include "request.hpp"
#include "response.hpp"
#include "socket.hpp"
#include <cstdlib>
#include <exception>
#include <inttypes.h>
#include <map>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <utility>

namespace http
{
class Server
{
  public:
    Server(void);
    Server(const Server &other);
    Server &operator=(const Server &other);
    ~Server(void);

    //    Member functions
    void listen(void);
    void stop(void);
    void pause(void);
    void init(const std::list< t_serverData > &configs, uint32_t timeout);

  private:
    struct timeval _timeout;
    std::map< int, t_serverData > _serverSet;
    std::map< int, std::list< t_serverData > > _serverSetByPort;
    std::map< int, std::pair< http::Request, t_serverData > > _requests;
    bool _run;
    int _epoll_fd;

    void _handleEpollout(Socket &sock, std::pair< http::Request, t_serverData > &data, struct epoll_event *event,
                         int epoll_fd);
    void _handleEpollin(Socket &sock, std::pair< http::Request, t_serverData > &data, const int epoll_fd,
                        struct epoll_event *event);
    void _handleReady(int epoll_fd, const int fd, struct epoll_event *event) throw(std::exception);
    void _watchFds(void);
    void _log(const http::Request &request, const http::Response &response) const;
    void _removeAcceptedFD(Socket &sock);
    t_serverData _getServerData(Socket &sock, std::string host);
};
} // namespace http
#endif
