/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:29:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/29 19:05:36 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <iostream>
#include <map>
#include <sys/types.h>

/******************************************************************************
 *               Constructeurs
 ******************************************************************************/
Request::Request( const std::string content )
{
	std::cout << content << std::endl;
}

Request::Request( const Request &other )
	: _method( other._method ), _uri( other._uri ), _protocol( other._protocol ), _host( other._host ),
	  _headers( other._headers )
{
}

Request &Request::operator=( const Request &other )
{
	_headers = other._headers;
	_host = other._host;
	_method = other._method;
	_protocol = other._protocol;
	_uri = other._uri;
	( void )other;
	return *this;
}

Request::~Request( void )
{
}

/******************************************************************************
 *               Fonctions membres
 ******************************************************************************/

std::string Request::method( void ) const
{
	return _method;
}

std::string Request::uri( void ) const
{
	return _uri;
}

std::string Request::protocol( void ) const
{
	return _protocol;
}

std::string Request::host( void ) const
{
	return _host;
}

std::string Request::header( const std::string key ) const
{
	std::map< std::string, std::string >::const_iterator found = _headers.find( key );
	if ( found != _headers.end() )
		return found->second;
	return "";
}