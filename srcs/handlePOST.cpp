/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 11:08:41 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/09 11:11:05 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "webserv.hpp"

http::Response handlePOST(const http::Request &request, const t_serverData &data)
{
    (void)request;
    (void)data;
    http::Response ret = http::Response(http::OK);
    ret.setBody("<h1>POST request OK</h1>", "text/html; charset=utf-8");
    return ret;
}