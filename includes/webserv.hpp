/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:47 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/03 12:21:53 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "config.hpp"
#include "handleRequest.hpp"
#include "parsing/parser.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "socket.hpp"
#include "statusCode.hpp"

#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iterator>
//#include <netinet/in.h>
//#include <netinet/ip.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/socket.h>
//#include <sys/stat.h>
#include "utils/ft_itoa.hpp"
#include <algorithm>
#include <csignal>
#include <dirent.h>
#include <map>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define SSTR(x) static_cast< std::ostringstream & >((std::ostringstream() << std::dec << x)).str()

#endif
