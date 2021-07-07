/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 17:34:47 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/07 18:47:58 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/******************************************************************************
 *			Constructeurs
 *******************************************************************************/
Response::Response(http::Status code) : _code(code)
{
}

Response::Response(const Response &other)
    : _code(other._code), _status(other._status), _headers(other._headers), _body(other._body)
{
}

Response &Response::operator=(const Response &other)
{
    _code = other._code;
    _body = other._body;
    _headers = other._headers;
    _status = other._status;
    return *this;
}

Response::~Response()
{
}

/********************************************************************************
 *			Setters
 ********************************************************************************/

void Response::setHeader(const std::string key, const std::string value)
{
    _headers[key] += value;
}

void Response::setBody(const std::string &content, const std::string mimetype)
{
    _body.first = content;
    _body.second = mimetype;
}

void Response::setCode(const http::Status code)
{
    _code = code;
}

/*******************************************************************************
 *			Fonctions membres
 *******************************************************************************/

std::string Response::toString() const
{
    return "";
}
