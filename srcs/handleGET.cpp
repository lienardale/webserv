/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/26 18:24:33 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include "response.hpp"
#include "webserv.hpp"
#include "handleRequest.hpp"
#include <string>

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
                _content += ("<li><a href=\"" + request.header("Path") + d_slashb + std::string(contents->d_name) +
                             d_slash + "\">" + (std::string(contents->d_name) + d_slash + "</a></li>\n"));
            d_slash = "";
        }
    }
    if (directory[directory.size() - 1] != '/')
        ret.setCode(http::MOVED_PERMANENTLY);
    closedir(dh);
    return (_content);
}

http::Response handleGET(const http::Request &request, const t_serverData &data, const t_locInfos &loc)
{
    std::fstream f;
    std::string file;
	std::string Location;
	std::string slash;

    http::Response ret = http::Response(http::OK);

    if (request.header("Path") == "/" || (loc._directory && !loc._index.empty()))
        file = data.root + request.header("Path") + loc._index;
    else
        file = data.root + request.header("Path"); // classic path request
    f.open(file.c_str(), std::ios::in);	
    if (((f.good() && !f.rdbuf()->in_avail()) || (!f.good() && !access(file.c_str(), F_OK))) && !emptyFile(&f))
    {
        if (f.good() && !f.rdbuf()->in_avail() && (!loc._directory || (loc._isDir && !loc._index.empty())))
        {
            ret.setCode(http::MOVED_PERMANENTLY);
			slash = (loc._directory) ? "" : "/";
			Location = (!loc._isDir) ? "http://" + request.header("Host") + request.header("Path") + "/" :  "http://" + request.header("Host") + request.header("Path") + slash + loc._index;
			ret.setHeader("Location", Location);
		}
		else if ((f.good() && !f.rdbuf()->in_avail()) && data.autoindex)
            ret.setBody(directoryListing(file, data, ret, request, loc), "text/html");
        else if (loc._directory)
            ret.setCode(http::NOT_FOUND);
        else
            ret.setCode(http::FORBIDDEN);
    }
    else if (f.good() && php_file(request.header("Path")))
        ret.setBodyCGI(cgi(request, data.locations.front(), data).getOutput()); // Cgi fct to modify and/or move
    else if (f.good())
        ret.setBody(std::string((std::istreambuf_iterator< char >(f)), std::istreambuf_iterator< char >()),
                    "text/html");
    // else if (loc._directory && !loc._index.empty() && !data.autoindex)
    //	ret.setCode(http::FORBIDDEN);
    else
        ret.setCode(http::NOT_FOUND);
    f.close();
    // ret.setBody(request.header("method") + " " + request.header("path") + " " + request.header("protocol"),
    //             "text/html; charset=utf-8");
    return ret;
}
