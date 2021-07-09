/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:59:02 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/09 11:22:30 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>

/*static bool methodAllowed(const std::string method, const t_serverData &data)
{
    std::string path1;

    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); ++it)
    {
        path1 = (it->path[it->path.size() - 1] == '/') ? it->path.substr(0, it->path.size() - 1) : it->path;
        if (_infos[1].find(path1) != std::string::npos && contains(it->methods, _infos[0]))
            return false;
    }
    return true;
}*/

http::Response handleRequest(const http::Request &request, const t_serverData &data)
{
    std::string method = request.header("Method");

    if (method == "GET")
        return handleGET(request, data);
    if (method == "POST")
        return handlePOST(request, data);
    if (method == "DELETE")
        return handleDELETE(request, data);
    return http::Response(http::METHOD_NOT_ALLOWED);
}