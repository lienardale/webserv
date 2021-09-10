/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/09/08 12:07:38 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include "handleRequest.hpp"
#include "response.hpp"
#include "webserv.hpp"
#include <string>

http::Response handleGET(const http::Request &request, const t_serverData &data, const t_locInfos &loc)
{
    std::fstream f;
    std::string file;
    std::string Location;
    std::string slash;
    std::string root;

    http::Response ret = http::Response(http::OK);

    if (request.header("Path") == "/" || (loc._directory && !loc._index.empty()))
        file = data.root.substr(0, data.root.size() - 1) + request.header("Path") + loc._index;
    else
        file = data.root.substr(0, data.root.size() - 1) + request.header("Path"); // classic path request
    f.open(file.c_str(), std::ios::in);
    if (((f.good() && !f.rdbuf()->in_avail()) || (!f.good() && !access(file.c_str(), F_OK))) && !emptyFile(&f))
    {
        if (f.good() && !f.rdbuf()->in_avail() && (!loc._directory || (loc._isDir && !loc._index.empty())))
        {
            ret.setCode(http::MOVED_PERMANENTLY);
            slash = (loc._directory) ? "" : "/";
            Location = (!loc._isDir) ? "http://" + request.header("Host") + loc._urlPath + "/"
                                     : "http://" + request.header("Host") + loc._urlPath + slash + loc._index;
            ret.setHeader("Location", Location);
        }
        else if ((f.good() && !f.rdbuf()->in_avail()) && data.autoindex)
            ret.setBody(directoryListing(file, data, ret, request, loc), "text/html");
        else if (loc._directory)
            ret.setCode(http::NOT_FOUND);
        else
            ret.setCode(http::FORBIDDEN);
    }
    else if (f.good() && !loc._fastcgiParam.empty() && php_file(file))
        ret.setBodyCGI(cgi(request, loc, data, file).getOutput());
    else if (f.good())
        ret.setBody(std::string((std::istreambuf_iterator< char >(f)), std::istreambuf_iterator< char >()),
                    mimeTypes(file, data));
    else
        ret.setCode(http::NOT_FOUND);
    f.close();
    return ret;
}
