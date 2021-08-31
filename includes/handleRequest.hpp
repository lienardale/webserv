/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:47:47 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/31 16:24:59 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLEMETHOD_HPP
#define HANDLEMETHOD_HPP

#include "parsing/dataStructure.hpp"
#include "request.hpp"
#include "response.hpp"

typedef struct	s_locInfos
{
	bool _isDir;
    bool _directory;
    std::string	_index;
	std::string	_content;
	std::string _locb;
	std::string _urlPath;
	std::string _fastcgiParam;
	std::string _uploadDir;
	bool cgi;
}				t_locInfos;

http::Response handleRequest(const http::Request &request, t_serverData &data);

http::Response handleGET(const http::Request &request, const t_serverData &data, const t_locInfos &loc);

http::Response handlePOST(const http::Request &request, const t_serverData &data, const t_locInfos &loc);

http::Response handleDELETE(const http::Request &request, const t_serverData &data);

bool php_file(std::string file);

bool cgiActivated(const std::string &file, const t_locInfos &loc);

bool emptyFile(std::fstream *f);


std::string directoryListing(std::string file, const t_serverData &data, http::Response &ret,
		const http::Request &request, const t_locInfos &loc);

std::string mimeTypes(std::string file, const t_serverData &data);

#endif
