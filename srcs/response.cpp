/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 17:34:47 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/12 14:00:22 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "statusCode.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <ios>
#include <map>
#include <sstream>
#include <string>

/******************************************************************************
 *              Outils
 ******************************************************************************/
static std::string statusLine(http::Status status)
{
    std::ostringstream oss;

    oss << "HTTP/1.1 ";

    std::string s = http::statusToReason(status);

    if (s.size())
        oss << status << " " << s << "\r\n";
    else
        oss << http::INTERNAL_SERVER_ERROR << " " << http::statusToReason(http::INTERNAL_SERVER_ERROR) << "\r\n";
    return oss.str();
}
/******************************************************************************
 *			Constructeurs
 *******************************************************************************/
http::Response::Response(void)
{
}

http::Response::Response(http::Status code) : _code(code)
{
}

http::Response::Response(const Response &other)
    : _code(other._code), _status(other._status), _headers(other._headers), _body(other._body)
{
}

http::Response &http::Response::operator=(const Response &other)
{
    _code = other._code;
    _body = other._body;
    _headers = other._headers;
    _status = other._status;
    return *this;
}

http::Response::~Response()
{
}

/********************************************************************************
 *			Setters
 ********************************************************************************/

void http::Response::setHeader(const std::string key, const std::string value)
{
    _headers[key] += value;
}

void http::Response::setBody(const std::string &content, const std::string mimetype)
{
    _body.first = content;
    _body.second = mimetype;
}

void http::Response::setCode(const http::Status code)
{
    _code = code;
}
/********************************************************************************
 *			Getters
 ********************************************************************************/
int http::Response::code(void) const
{
    return _code;
}
/*******************************************************************************
 *			Fonctions membres
 *******************************************************************************/

std::string http::Response::toString() const
{
    std::ostringstream oss;
    time_t now = time(0);
    char *dt = ctime(&now);

    oss << statusLine(_code) << ""
        << "Accept-Ranges: bytes\r\n";

    oss << "Server: NGINX -2.0\r\n";
    oss << "Date: " << dt;

    for (std::map< std::string, std::string >::const_iterator it = _headers.begin(); it != _headers.end(); it++)
        oss << it->first << ": " << it->second << "\r\n";

    if (_body.first.size())
    {
        oss << "Content-Length: " << _body.first.size() << "\r\n";
        oss << "Content-Type: " << _body.second << "\r\n";
        oss << std::endl << _body.first;
    }
    else if (_body.first.empty() && _code >= 400)
    {
        std::string r = "<h1>" + http::statusToReason(_code) + "</h1>";
        oss << "Content-Length: " << r.size() << "\r\n";
        oss << "Content-Type: "
            << "text/html\r\n";
        oss << std::endl << r;
    }
    else
        oss << std::endl;
    return oss.str();
}

std::string http::Response::toString(const std::map< int, std::string > &errorPages)
{
    std::map< int, std::string >::const_iterator found = errorPages.find(_code);

    if (found != errorPages.end())
        setBody(found->second, "text/html");
    return toString();
}
