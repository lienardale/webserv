/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:29:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/01 17:03:36 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parsingExceptions.hpp"
#include "parsing/utils.hpp"
#include "webserv.hpp"
#include <cctype>
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

static std::list<std::string> ft_split(std::string str, std::string toFind)
{
    std::list<std::string> l;
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

static void extractHeader(std::map<std::string, std::string> &headers, std::string content) throw(ParsingException)
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

/******************************************************************************
 *               Constructeurs
 ******************************************************************************/
Request::Request(void)
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
            std::list<std::string> query = ft_split(s, " ");
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
    : _method(other._method), _uri(other._uri), _protocol(other._protocol), _host(other._host), _headers(other._headers)
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

std::map<std::string, std::string> Request::getHeader() const
{
    return _headers;
}
/******************************************************************************
 *               Fonctions membres
 ******************************************************************************/
std::string Request::header(const std::string key) const
{
    std::map<std::string, std::string>::const_iterator found = _headers.find(key);
    if (found != _headers.end())
        return found->second;
    return "";
}

/********************************************************************************
 *					Operator overloading
 *******************************************************************************/
std::ostream &operator<<(std::ostream &os, const Request &r)
{
    os << "Method: " << r.method() << std::endl;
    os << "Uri: " << r.uri() << std::endl;
    os << "Protocol: " << r.protocol() << std::endl;

    std::map<std::string, std::string> headers = r.getHeader();
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        os << it->first << ": " << it->second << std::endl;
    return os;
}