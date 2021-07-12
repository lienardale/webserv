/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:59:02 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/12 16:01:52 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <algorithm>

static std::list< t_locationData >::iterator findData(const http::Request &request, t_serverData &data)
{
    std::string path = request.header("path");
    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); it++)
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

// set autoindex true if autoindex is activated in a parent's location
static void locAutoindex(const http::Request &request, t_serverData &data)
{
    std::string path1;

    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); ++it)
    {
        path1 = (it->path[it->path.size() - 1] == '/') ? it->path.substr(0, it->path.size() - 1) : it->path;
        if (it->autoindex && request.header("Path").find(path1) != std::string::npos)
            data.autoindex = true;
    }
}

// check if path/
static bool directory(const std::string &name)
{
    return (*name.rbegin() == '/') ? true : false;
}

static bool fileExists(const http::Request &request, t_serverData data, const std::string &name)
{
    bool exists;

    std::ifstream f((data.root + request.header("Path") + "/" + name).c_str());
    exists = f.good();
    f.close();
    return exists;
}

// check which file to use in index
// set index if set in a parent's location
static void locIndex(const http::Request &request, t_serverData &data, t_locationData *found)
{
    std::string path1;

    for (std::list< std::string >::iterator it = data.index.begin(); it != data.index.end(); ++it)
    {
        if (!it->empty() && fileExists(request, data, *it))
        {
            found->_index = *it;
            break;
        }
    }

    for (std::list< t_locationData >::iterator it1 = data.locations.begin(); it1 != data.locations.end(); ++it1)
    {
        path1 = (it1->path[it1->path.size() - 1] == '/') ? it1->path.substr(0, it1->path.size() - 1) : it1->path;
        if (!it1->index.front().empty() && request.header("Path").find(path1) != std::string::npos)
        {
            for (std::list< std::string >::iterator it2 = it1->index.begin(); it2 != it1->index.end(); ++it2)
            {
                if (!it2->empty() && fileExists(request, data, *it2))
                {
                    found->_index = *it2;
                    break;
                }
            }
        }
    }

    std::ifstream f((data.root + request.header("Path") + "/" + found->_index).c_str());
    found->_isDir = (f.good() && !f.rdbuf()->in_avail()) ? true : false;
    f.close();
}

http::Response handleRequest(const http::Request &request, t_serverData &data)
{
    std::string method = request.header("Method");
    if (method != "GET" && method != "POST" && method != "DELETE")
        return http::Response(http::METHOD_NOT_ALLOWED);

    std::list< t_locationData >::iterator found = findData(request, data);
    
    found->_directory = directory(request.header("Path"));
    locIndex(request, data, &(*found));
    locAutoindex(request, data);
    if (found == data.locations.end())
        return http::Response(http::NOT_FOUND);
    
    bool isAllowed = isMethodAllowed(method, *found);
    if (method == "GET" && isAllowed)
        return handleGET(request, *found);
    if (method == "POST" && isAllowed)
        return handlePOST(request, *found);
    if (method == "DELETE" && isAllowed)
        return handleDELETE(request, *found);
    return http::Response(http::METHOD_NOT_ALLOWED);
}
