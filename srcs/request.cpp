/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:29:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/07 18:42:07 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "parsing/parsingExceptions.hpp"
#include "parsing/utils.hpp"
#include "webserv.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <ostream>
#include <string>
#include <sys/types.h>

/******************************************************************************
 *				Outils
 *****************************************************************************/

static std::list< std::string > ft_split(std::string str, std::string toFind)
{
    std::list< std::string > l;
    std::string::size_type pos;
    std::string s;

    while ((pos = str.find(toFind)) != std::string::npos && str.begin() != str.end())
    {
        l.push_back(str.substr(0, pos));
        str.erase(str.begin(), str.begin() + pos + 1);
    }
    if (pos == std::string::npos)
        l.push_back(str.substr(0, pos));
    return l;
}

static void extractHeader(std::map< std::string, std::string > &headers, std::string content) throw(ParsingException)
{
    std::string key, value;
    std::string::size_type pos;

    pos = content.find(": ");
    if (pos != std::string::npos)
    {
        key = content.substr(0, pos);
        value = content.substr(pos + 2, &*content.rend() - &content[pos + 2]);
        if (headers.find(key) != headers.end())
            headers[key] += "," + value;
        else
            headers[key] = value;
    }
    else
        throw BadRequest("Wrong key value syntax");
}

static bool checkQuery(std::map< std::string, std::string > &headers)
{
    std::string method = headers["Method"];
    std::string protocol = headers["Protocol"];

    return (method == "GET" || method == "POST" || method == "DELETE") && protocol == "HTTP/1.1";
}

static void extract(std::map< std::string, std::string > &headers, std::string content) throw(ParsingException)
{
    std::map< std::string, std::string >::iterator found = headers.find("Method");

    if (found != headers.end())
        extractHeader(headers, content);
    else
    {
        std::list< std::string > splitted = ft_split(content, " ");
        if (splitted.size() == 3)
        {
            headers["Method"] = splitted.front();
            headers["Path"] = *++splitted.begin();
            headers["Protocol"] = splitted.back();
            if (!checkQuery(headers))
                throw BadRequest("Wrong query format");
        }
        else
            throw BadRequest("Wrong query format");
    }
}

static void extractBody(std::map< std::string, std::string > &headers, std::string &buffer,
                        bool &finish) throw(ParsingException)
{
    std::string body;
    std::map< std::string, std::string >::iterator found = headers.find("Content-Length");

    if (!finish && found != headers.end())
    {
        int len = std::atoi(found->second.c_str());
        if (len <= 0)
            throw BadRequest("Wrong Content-Length value");

        found = headers.find("Body");

        if (static_cast< int >(body.size() + buffer.size()) > len)
            throw BadRequest("Wrong body size");

        headers["Body"] += buffer;
        finish = static_cast< int >(headers["Body"].size()) == len;
        if (finish && headers.find("Host") == headers.end())
            throw BadRequest("No Host in header");

        buffer.clear();
    }
    else
        throw BadRequest("No Content-Length");
}
/******************************************************************************
 *               Constructeurs
 ******************************************************************************/
http::Request::Request(void) : _isBody(false), _finish(false)
{
}

http::Request::Request(const Request &other)
    : _isBody(other._isBody), _finish(other._finish), _method(other._method), _uri(other._uri),
      _protocol(other._protocol), _host(other._host), _headers(other._headers)
{
}

http::Request &http::Request::operator=(const http::Request &other)
{
    (void)other;

    _headers = other._headers;
    _host = other._host;
    _method = other._method;
    _protocol = other._protocol;
    _uri = other._uri;
    _finish = other._finish;
    return *this;
}

http::Request::~Request(void)
{
}

/******************************************************************************
 *               Getters
 ******************************************************************************/

std::string http::Request::method(void) const
{
    return _method;
}

std::string http::Request::uri(void) const
{
    return _uri;
}

std::string http::Request::protocol(void) const
{
    return _protocol;
}

std::map< std::string, std::string > http::Request::getHeader() const
{
    return _headers;
}

bool http::Request::isFinished() const
{
    return _finish;
}
/******************************************************************************
 *               Fonctions membres
 ******************************************************************************/
std::string http::Request::header(const std::string key) const
{
    std::map< std::string, std::string >::const_iterator found = _headers.find(key);
    if (found != _headers.end())
        return found->second;
    return "";
}

void http::Request::parse(std::string content) throw(ParsingException)
{
    std::string::size_type pos;
    std::string val;

    _buffer += content;
    while (!_isBody && (pos = _buffer.find("\r\n")) != std::string::npos)
    {
        val = _buffer.substr(0, pos);
        if (!(_isBody = val.size() == 0))
            extract(_headers, val);
        _buffer = _buffer.substr(pos + 2, _buffer.size() - (pos + 2));
    }
    if (_isBody && _buffer.size())
        extractBody(_headers, _buffer, _finish);
    else if (_isBody && _buffer.size() == 0)
    {
        _finish = true;
        if (_headers.find("Host") == _headers.end())
            throw BadRequest("No Host in header");
    }
}

/********************************************************************************
 *					Operator overloading
 *******************************************************************************/
std::ostream &operator<<(std::ostream &os, const http::Request &r)
{
    std::map< std::string, std::string > headers = r.getHeader();
    for (std::map< std::string, std::string >::iterator it = headers.begin(); it != headers.end(); it++)
        os << it->first << ": " << it->second << std::endl;
    return os;
}
