/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:29:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/09/08 12:07:44 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "parsing/parsingExceptions.hpp"
#include "parsing/utils.hpp"
#include "webserv.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector>

/******************************************************************************
 *				Outils
 *****************************************************************************/

static std::vector< std::string > ft_split(std::string str, std::string toFind)
{
    std::vector< std::string > v(3);
    std::string::size_type pos;
    std::string s;
    int i = 0;

    while ((pos = str.find(toFind)) != std::string::npos && str.begin() != str.end())
    {
        v[i] = str.substr(0, pos);
        str.erase(str.begin(), str.begin() + pos + 1);
        i++;
    }
    v[i] = str.substr(0, pos);
    return v;
}

/******************************************************************************
 *               Constructeurs
 ******************************************************************************/
http::Request::Request(void) : _isBody(false), _finish(false), _isBodyTooLarge(false)
{
}

http::Request::Request(const Request &other)
    : _isBody(other._isBody), _finish(other._finish), _isBodyTooLarge(other._isBodyTooLarge), _buffer(other._buffer),
      _headers(other._headers)
{
}

http::Request &http::Request::operator=(const http::Request &other)
{
    (void)other;

    _headers = other._headers;
    _finish = other._finish;
    _buffer = other._buffer;
    _isBody = other._isBody;
    _isBodyTooLarge = other._isBodyTooLarge;
    return *this;
}

http::Request::~Request(void)
{
}

/******************************************************************************
 *               Getters
 ******************************************************************************/
std::map< std::string, std::string > http::Request::getHeader() const
{
    return _headers;
}

bool http::Request::isFinished() const
{
    return _finish;
}

bool http::Request::keepAlive() const
{
    std::map< std::string, std::string >::const_iterator found = _headers.find("connection");
    if (found != _headers.end())
        return found->second != "close";
    return true;
}

bool http::Request::isBodyTooLarge(void) const
{
    return _isBodyTooLarge;
}
/******************************************************************************
 *               Setters
 ******************************************************************************/

void http::Request::setHeaderPath(std::string path)
{
    _headers["path"] = path;
}

/******************************************************************************
 *               Fonctions membres
 ******************************************************************************/
std::string http::Request::header(const std::string key) const
{
    std::string k = key;
    std::transform(k.begin(), k.end(), k.begin(), ::tolower);
    std::map< std::string, std::string >::const_iterator found = _headers.find(k);
    if (found != _headers.end())
        return found->second;
    return "";
}

static std::string _cleanPath(std::string path)
{
    std::vector< std::string > res = ft_split(path, "?");
    return res.front();
}

static std::string _cleanQuery(std::string path)
{

    std::vector< std::string > res = ft_split(path, "?");
    if (res.size() > 1)
        return *++res.begin();
    return "";
}

void http::Request::_extract(std::string &content)
{
    std::string method = header("method");
    if (method.size())
        _extractHeader(content);
    else
    {
        std::vector< std::string > splitted = ft_split(content, " ");
        if (splitted.size() == 3)
        {
            _headers["method"] = splitted.front();
            _headers["path"] = _cleanPath(splitted[1]);
            _headers["query"] = _cleanQuery(splitted[1]);
            _headers["protocol"] = ft_split(splitted.back(), "\r").front();
            if (_headers["protocol"] != "HTTP/1.1")
                throw BadRequest("Wrong query protocol");
            if (splitted.front() != "GET" && splitted.front() != "DELETE" && splitted.front() != "POST")
                throw BadRequest("Wrong http method");
        }
        else
            throw BadRequest("Wrong query format");
    }
}

void http::Request::_extractBody(int maxBodySize)
{
    std::string body = header("body");
    std::string host = header("host");
    int contentLength = std::atoi(header("content-length").c_str());

    if (host.empty())
        BadRequest("No host in header");
    if (static_cast< int >(body.size() + _buffer.size()) > contentLength)
        throw BadRequest("Wrong body size");
    if (static_cast< int >(body.size() + _buffer.size()) > maxBodySize)
        _isBodyTooLarge = true;
    _headers["body"] += _buffer;
    _buffer.clear();
    _finish = static_cast< int >(_headers["body"].size()) == contentLength;
}

void http::Request::_extractHeader(std::string &content)
{
    std::string key, value;
    std::string::size_type pos;

    pos = content.find(": ");
    if (pos != std::string::npos)
    {
        key = content.substr(0, pos);
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        value = content.substr(pos + 2, &*content.rend() - &content[pos + 2]);
        _headers[key] = ft_split(value, "\r").front();
    }
    else
        throw BadRequest("Wrong key value syntax");
}

void http::Request::parse(const std::string &content, int maxBodySize)
{
    std::string::size_type pos;
    std::string val;

    _buffer += content;
    while (!_isBody && (pos = _buffer.find("\n")) != std::string::npos)
    {
        val = _buffer.substr(0, pos);
        if (!(_isBody = (val == "\r") || val.empty()))
            _extract(val);
        _buffer = _buffer.substr(pos + 1, _buffer.size() - (pos + 1));
    }

    std::string contentLength = header("content-length");
    if (_isBody && !_buffer.empty() && !contentLength.empty())
        _extractBody(maxBodySize);
    else if (_isBody && contentLength.empty())
    {
        _finish = true;
        if (_headers.find("host") == _headers.end())
            throw BadRequest("No Host in header");
    }
}

void http::Request::clear(void)
{
    _headers.clear();
    _buffer.clear();
    _finish = false;
    _isBody = false;
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
