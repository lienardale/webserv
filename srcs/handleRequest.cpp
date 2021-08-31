/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard@student.42.fr <alienard>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:59:02 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/30 17:21:30 by alienard@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/dataStructure.hpp"
#include "webserv.hpp"
#include <algorithm>

std::string mimeTypes(std::string file, const t_serverData &data)
{
	std::string ext;

	for (std::string::reverse_iterator it = file.rbegin(); it != file.rend(); ++it)
	{
		if (*it == '.')
			break;
		ext += *it;
	}
	reverse(ext.begin(), ext.end());
	return (*data.mimeTypes)[ext].empty() ? "application/octet-stream" : (*data.mimeTypes)[ext];
}

bool	emptyFile(std::fstream *f)
{
	f->seekg (0, f->end);
	int length = f->tellg();
	f->seekg (0, f->beg);
	if (length == 0)
		return true;
	return false;
}

std::string directoryListing(std::string file, const t_serverData &data, http::Response &ret,
		const http::Request &request, const t_locInfos &loc)
{
	DIR *dh;
	DIR *is_dir;
	struct dirent *contents;
	std::string directory = file;
	std::fstream f;
	std::string d_slash;
	std::string d_slashb;
	std::string _content;

	if (!(dh = opendir(directory.c_str())))
		ret.setCode(http::NOT_FOUND);
	else
	{
		_content += ("<h1>Index of " + request.header("Path") + "</h1>\n");
		while ((contents = readdir(dh)) != NULL)
		{
			d_slashb = (loc._directory) ? "" : "/";
			if ((is_dir = opendir((data.root + request.header("Path") + std::string(contents->d_name)).c_str())))
				d_slash = "/";
			closedir(is_dir);
			if (std::string(contents->d_name) != ".")
				_content += ("<li><a href=\"" + loc._urlPath + d_slashb + std::string(contents->d_name) +
						d_slash + "\">" + (std::string(contents->d_name) + d_slash + "</a></li>\n"));
			d_slash = "";
		}
	}
	if (directory[directory.size() - 1] != '/')
		ret.setCode(http::MOVED_PERMANENTLY);
	closedir(dh);
	return (_content);
}

bool php_file(std::string file)
{
    std::string ext;

    for (std::string::reverse_iterator it = file.rbegin(); it != file.rend(); ++it)
    {
        if (*it == '.')
            break;
        ext += *it;
    }
    if (ext == "php")
        return true;
    return false;
}

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
        path1 = (it->path[it->path.size() - 1] == '/' && it->path != "/") ? it->path.substr(0, it->path.size() - 1)
                                                                          : it->path;
        if (it->autoindex != data.autoindex && request.header("Path").find(path1) != std::string::npos)
            data.autoindex = it->autoindex;
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

std::string pathModifiedIfRoot(std::string path, t_serverData &data, t_locInfos *loc)
{
    std::string path1;
    std::string root;

    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); ++it)
    {
        path1 = it->path;
        if (*path1.rbegin() != '/')
            path1.push_back('/');
        if (!it->root.empty() && path.find(path1) != std::string::npos && loc->_locb != path1)
        {
            root = (it->root[it->root.size() - 1] == '/' && it->root != "/") ? it->root.substr(0, it->root.size() - 1)
                                                                             : it->root;
            loc->_locb = path1;
            return path.replace(path.find(it->path), it->path.size(), root);
        }
    }
    return "";
}

bool methodAllowed(const http::Request &request, t_serverData &data)
{
    std::string path1;

    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); ++it)
    {
        // std::cout << request.header("Path") << std::endl;
        path1 = (it->path[it->path.size() - 1] == '/') ? it->path.substr(0, it->path.size() - 1) : it->path;
        if (request.header("Path").find(path1) != std::string::npos && contains(it->methods, request.header("Method")))
            return false;
    }
    return true;
}

void locCgi(const http::Request &request, t_serverData &data, t_locInfos *loc)
{
    std::string path1;

    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); ++it)
    {
        path1 = (it->path[it->path.size() - 1] == '/' && it->path != "/") ? it->path.substr(0, it->path.size() - 1)
                                                                          : it->path;
		if (!it->fastcgi_param["fastcgi_param"].empty() && request.header("Path").find(path1) != std::string::npos)
			loc->_fastcgiParam = it->fastcgi_param["fastcgi_param"];
    }
}

http::Response handleRequest(const http::Request &requestHeader, t_serverData &data)
{
    std::string method = requestHeader.header("Method");
    t_locInfos loc;
    std::string path;

    http::Request request(requestHeader);
    if (method != "GET" && method != "POST" && method != "DELETE")
        return http::Response(http::METHOD_NOT_ALLOWED);
    if (*request.header("Path").begin() != '/')
        return http::Response(http::FORBIDDEN);
    loc._urlPath = request.header("Path");
    while (!(path = pathModifiedIfRoot(request.header("Path"), data, &loc)).empty())
        request.setHeaderPath(path);
    loc._directory = directory(request.header("Path"));
    locIndex(request, data, &loc);
    locAutoindex(request, data);
	locCgi(request, data, &loc);
    if (method == "GET" && methodAllowed(request, data))
        return handleGET(request, data, loc);
    if (method == "POST" && methodAllowed(request, data))
        return handlePOST(request, data, loc);
    if (method == "DELETE" && methodAllowed(request, data))
        return handleDELETE(request, data);
    return http::Response(http::METHOD_NOT_ALLOWED);
}
