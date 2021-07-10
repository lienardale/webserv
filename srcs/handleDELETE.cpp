/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/10 11:02:20 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "webserv.hpp"

http::Response handleDELETE(const http::Request &request, const t_locationData &data)
{
    (void)request;
    (void)data;
    http::Response ret = http::Response(http::OK);
    ret.setBody("<h1>DELETE request OK</h1>", "text/html; charset=utf-8");
    return ret;
}