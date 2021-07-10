/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:59:02 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/10 10:52:22 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static std::list< t_locationData >::const_iterator findData(const http::Request &request, const t_serverData &data)
{
    std::string path = request.header("path");
    for (std::list< t_locationData >::const_iterator it = data.locations.begin(); it != data.locations.end(); it++)
        if (it->path == path)
            return it;
    return data.locations.end();
}

static bool isMethodAllowed(const std::string &method, const t_locationData &data)
{
    return std::binary_search(data.methods.begin(), data.methods.end(), method);
}

http::Response handleRequest(const http::Request &request, const t_serverData &data)
{
    std::string method = request.header("Method");
    if (method != "GET" && method != "POST" && method != "DELETE")
        return http::Response(http::METHOD_NOT_ALLOWED);

    std::list< t_locationData >::const_iterator found = findData(request, data);
    if (found == data.locations.end())
        return http::Response(http::NOT_FOUND);

    bool isAllowed = isMethodAllowed(method, *found);
    if (method == "GET" && isAllowed)
        return handleGET(request, data);
    if (method == "POST" && isAllowed)
        return handlePOST(request, data);
    if (method == "DELETE" && isAllowed)
        return handleDELETE(request, data);
    return http::Response(http::METHOD_NOT_ALLOWED);
}