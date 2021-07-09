/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/09 20:30:43 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "webserv.hpp"

http::Response handleGET(const http::Request &request, const t_serverData &data)
{
    (void)request;
    (void)data;

    http::Response ret = http::Response(http::OK);
    ret.setBody(request.header("method") + " " + request.header("path") + " " + request.header("protocol"),
                "text/html; charset=utf-8");
    return ret;
}