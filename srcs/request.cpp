/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:29:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/05 18:46:07 by dboyer           ###   ########.fr       */
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
        }
        else
            throw BadRequest("No query informations");
    }
}

static void extractBody(std::map< std::string, std::string > &headers, std::string &buffer,
                        bool &finish) throw(ParsingException)
{
    std::map< std::string, std::string >::iterator found = headers.find("Body");

    if (found != headers.end())
    {
        std::string body = found->second;
        found = headers.find("Content-Length");
        if (found != headers.end())
        {
            int len = std::atoi(found->second.c_str());
            if (len <= 0)
                throw BadRequest("Wrong content length value");
            if (static_cast< int >(body.size() + buffer.size()) <= len)
                headers["Body"] += buffer;
            else
            {
                headers["Body"] += buffer.substr(0, std::abs(static_cast< int >(body.size()) - len));
                finish = true;
            }
        }
    }
    else
        headers["Body"] += buffer;
    buffer.clear();
}
/******************************************************************************
 *               Constructeurs
 ******************************************************************************/
Request::Request(void) : _isBody(false), _finish(false)
{
}

Request::Request(std::string content) throw(ParsingException)
{
    int n = 0;
    bool body = false;
    std::string::size_type pos;

    while ((pos = content.find("\r\n")) != std::string::npos)
    {
        std::string s = content.substr(0, pos);
        content.erase(content.begin(), content.begin() + pos + 2);

        if (n == 0)
        {
            std::list< std::string > query = ft_split(s, " ");
            if (query.size() != 3)
                throw BadRequest("Bad request");
            _method = *query.begin();
            _uri = *++query.begin();
            _protocol = *query.rbegin();
        }
        else
        {
            if (s.size() > 2 && !body)
                extractHeader(_headers, s);
            else if (s.size() <= 2 && !body)
                body = true;
            else
                _headers["Body"] += s;
        }
        n++;
    }
    if (body && content.size())
        _headers["Body"] += content;
}

Request::Request(const Request &other)
    : _isBody(other._isBody), _method(other._method), _uri(other._uri), _protocol(other._protocol), _host(other._host),
      _headers(other._headers)
{
}

Request &Request::operator=(const Request &other)
{
    (void)other;

    _headers = other._headers;
    _host = other._host;
    _method = other._method;
    _protocol = other._protocol;
    _uri = other._uri;
    return *this;
}

Request::~Request(void)
{
}

/******************************************************************************
 *               Getters
 ******************************************************************************/

std::string Request::method(void) const
{
    return _method;
}

std::string Request::uri(void) const
{
    return _uri;
}

std::string Request::protocol(void) const
{
    return _protocol;
}

std::map< std::string, std::string > Request::getHeader() const
{
    return _headers;
}

bool Request::isFinished() const
{
    return _finish;
}
/******************************************************************************
 *               Fonctions membres
 ******************************************************************************/
std::string Request::header(const std::string key) const
{
    std::map< std::string, std::string >::const_iterator found = _headers.find(key);
    if (found != _headers.end())
        return found->second;
    return "";
}

void Request::parse(std::string content) throw(ParsingException)
{
    std::string::size_type pos;
    std::string val;

    _buffer += content;
    if (!_isBody)
    {
        while ((pos = _buffer.find("\r\n")) != std::string::npos)
        {
            val = _buffer.substr(0, pos);
            _buffer = _buffer.substr(pos + 2, _buffer.size() - (pos + 2));
            if (val.size() && !_isBody)
                extract(_headers, val);
            else if (val.size() == 0 && !_isBody)
                _isBody = true;
        }
    }
    else if (_isBody && _buffer.size())
        extractBody(_headers, _buffer, _finish);
}

/********************************************************************************
 *					Operator overloading
 *******************************************************************************/
std::ostream &operator<<(std::ostream &os, const Request &r)
{
    std::map< std::string, std::string > headers = r.getHeader();
    for (std::map< std::string, std::string >::iterator it = headers.begin(); it != headers.end(); it++)
        os << it->first << ": " << it->second << std::endl;
    return os;
}
