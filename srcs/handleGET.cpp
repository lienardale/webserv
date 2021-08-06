/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/06 15:16:16 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include "handleRequest.hpp"
#include "response.hpp"
#include "webserv.hpp"
#include <string>

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
			Location = (!loc._isDir) ? "http://" + request.header("Host") + loc._urlPath + "/" :  "http://" + request.header("Host") + loc._urlPath + slash + loc._index;
			ret.setHeader("Location", Location);
		}
		else if ((f.good() && !f.rdbuf()->in_avail()) && data.autoindex)
			ret.setBody(directoryListing(file, data, ret, request, loc), mimeTypes(file, data));
		else if (loc._directory)
			ret.setCode(http::NOT_FOUND);
		else
			ret.setCode(http::FORBIDDEN);
	}	
	else if (f.good())
		ret.setBody(std::string((std::istreambuf_iterator< char >(f)), std::istreambuf_iterator< char >()), mimeTypes(file, data));
	else
		ret.setCode(http::NOT_FOUND);
	f.close();
	return ret;
}
