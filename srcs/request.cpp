/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:29:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/29 18:42:25 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>
#include <sys/types.h>

/******************************************************************************
 *               Constructeurs
 ******************************************************************************/
Request::Request( const std::string content )
{
}

Request::Request( const Request &other )
	: _method( other._method ), _uri( other._uri ), _protocol( other._protocol ), _host( other._host ),
	  _headers( other._headers )
{
}

Request &Request::operator=( const Request &other )
{
	*this = other;
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