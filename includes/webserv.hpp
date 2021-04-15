/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:47 by dboyer            #+#    #+#             */
/*   Updated: 2021/04/15 11:49:17 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8080
typedef int SOCKET;

void hello_world(void);

#endif
