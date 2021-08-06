/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePOST.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 11:08:41 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/04 15:44:36 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include "response.hpp"
#include "webserv.hpp"

http::Response handlePOST(const http::Request &request, const t_serverData &data, const t_locInfos &loc)
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
	if ((f.good() && !f.rdbuf()->in_avail()) || (!f.good() && !access(file.c_str(), F_OK)))
	{
		if (f.good() && !f.rdbuf()->in_avail() && (!loc._directory || (loc._isDir && !loc._index.empty())))
		{
			ret.setCode(http::MOVED_PERMANENTLY);
			slash = (loc._directory) ? "" : "/";
			Location = (!loc._isDir) ? "http://" + request.header("Host") + loc._urlPath + "/" :  "http://" + request.header("Host") + loc._urlPath + slash + loc._index;
			ret.setHeader("Location", Location);
		}
		else if (loc._directory)
			ret.setCode(http::METHOD_NOT_ALLOWED);
		else
			ret.setCode(http::FORBIDDEN);
	}
	else if (f.good() && php_file(request.header("Path")))
	{
		ret.setHeader("Transfer-Encoding", "chunked");
		ret.setBodyCGI(cgi(request, loc, data).getOutput()); // Cgi fct to modify and/or move
	}
	else if (f.good())
		ret.setCode(http::METHOD_NOT_ALLOWED);
	else
		ret.setCode(http::NOT_FOUND);
	f.close();
	return ret;
}
