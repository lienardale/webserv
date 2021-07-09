/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/09 15:15:15 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "webserv.hpp"

http::Response handleGET(const http::Request &request, const t_serverData &data)
{
    (void)request;
    (void)data;

    std::cout << data << std::endl;
    http::Response ret = http::Response(http::OK);
    ret.setBody("<h1>GET request OK</h1>", "text/html; charset=utf-8");
    return ret;
}