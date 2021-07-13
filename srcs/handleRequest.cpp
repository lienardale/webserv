/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:59:02 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/12 18:20:31 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <algorithm>

/*
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
*/

/*
 * Generic function to find if an element of any type exists in list
 */
template < typename T >

bool contains(std::list< T > &listOfElements, const T &element)
{
    // Find the iterator if element in li st
    typename std::list< T >::iterator it = std::find(listOfElements.begin(), listOfElements.end(), element);
    // return if iterator points to end or not. It points to end then it means
    // element does not exists in list
    return it != listOfElements.end();
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
static void locIndex(const http::Request &request, t_serverData &data, t_locInfos *loc)
{
    std::string path1;

    for (std::list< std::string >::iterator it = data.index.begin(); it != data.index.end(); ++it)
    {
        if (!it->empty() && fileExists(request, data, *it))
        {
            loc->_index = *it;
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
                    loc->_index = *it2;
                    break;
                }
            }
        }
    }

    std::ifstream f((data.root + request.header("Path") + "/" + loc->_index).c_str());
    loc->_isDir = (f.good() && !f.rdbuf()->in_avail()) ? true : false;
    f.close();
}

bool	methodAllowed(const http::Request &request, t_serverData &data)
{
    std::string path1;

    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); ++it)
    {
        path1 = (it->path[it->path.size() - 1] == '/') ? it->path.substr(0, it->path.size() - 1) : it->path;
        if (request.header("Path").find(path1) != std::string::npos && contains(it->methods, request.header("Method")))
            return false;
    }
    return true;
}

http::Response handleRequest(const http::Request &request, t_serverData &data)
{
    std::string method = request.header("Method");
    t_locInfos loc;

    if (method != "GET" && method != "POST" && method != "DELETE")
        return http::Response(http::METHOD_NOT_ALLOWED);
    loc._directory = directory(request.header("Path"));
    locIndex(request, data, &loc);
    locAutoindex(request, data);
    if (method == "GET" && methodAllowed(request, data))
        return handleGET(request, data, loc);
    if (method == "POST" && methodAllowed(request, data))
        return handlePOST(request, data);
    if (method == "DELETE" && methodAllowed(request, data))
        return handleDELETE(request, data);
    return http::Response(http::METHOD_NOT_ALLOWED);
}
