/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:59:02 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/10 17:03:17 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <algorithm>

static std::list< t_locationData >::const_iterator findData(const http::Request &request, const t_serverData &data)
{
    std::string path = request.header("path");
    for (std::list< t_locationData >::const_iterator it = data.locations.begin(); it != data.locations.end(); it++)
    {
        if ((path.find(it->path) != std::string::npos && it->path != "/") || it->path == path)
            return it;
        if (it->path == "/" && std::count(path.begin(), path.end(), '/') == 1 &&
            std::count(path.begin(), path.end(), '.') >= 1)
            return it;
    }
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

    std::cout << std::endl << *found << std::endl;
    bool isAllowed = isMethodAllowed(method, *found);
    if (method == "GET" && isAllowed)
        return handleGET(request, *found);
    if (method == "POST" && isAllowed)
        return handlePOST(request, *found);
    if (method == "DELETE" && isAllowed)
        return handleDELETE(request, *found);
    return http::Response(http::METHOD_NOT_ALLOWED);
}
