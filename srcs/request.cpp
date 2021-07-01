/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:29:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/01 12:36:15 by dboyer           ###   ########.fr       */
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
#include <string>
#include <sys/types.h>

/******************************************************************************
 *				Outils
 *****************************************************************************/
/*static std::string ft_str_trim( std::string str, char c )
{
	size_t first = str.find_first_not_of( c );
	size_t last = str.find_last_not_of( c );
	if ( first != std::string::npos && last != std::string::npos )
		return str.substr( first, ( last - first + 1 ) );
	return "";
}*/

static std::list< std::string > ft_split( std::string str, std::string toFind )
{
	std::list< std::string > l;
	std::string::size_type pos;
	std::string s;

	while ( ( pos = str.find( toFind ) ) != std::string::npos && str.begin() != str.end() )
	{
		l.push_back( str.substr( 0, pos ) );
		str.erase( str.begin(), str.begin() + pos + 1 );
	}
	if ( pos == std::string::npos )
		l.push_back( str.substr( 0, pos ) );
	return l;
}
/*

 static void ft_clean( std::string &str, std::string toRemove )
 {
	 std::string::size_type pos;
	 while ( ( pos = str.find( toRemove ) ) != std::string::npos )
		 str.erase( str.begin() + pos );
 }*/

static void extractHeader( std::map< std::string, std::string > &headers,
						   std::string content ) throw( ParsingException )
{

	std::string key, value;
	std::string::size_type pos;

	pos = content.find( ": " );
	if ( pos != std::string::npos )
	{
		key = content.substr( 0, pos );
		value = content.substr( pos + 2, &*content.rend() - &content[ pos + 2 ] );

		if ( headers.find( key ) != headers.end() )
			headers[ key ] += "," + value;
		else
			headers[ key ] = value;
	}
	else
		throw BadRequest( "Wrong key value syntax" );
}

/******************************************************************************
 *               Constructeurs
 ******************************************************************************/
Request::Request( std::string &content ) throw( ParsingException )
{
	int n = 0;
	bool body = false;
	std::string::size_type pos;

	while ( ( pos = content.find( "\r\n" ) ) != std::string::npos )
	{
		std::string s = content.substr( 0, pos );
		content.erase( content.begin(), content.begin() + pos + 2 );

		if ( n == 0 )
		{
			std::list< std::string > query = ft_split( s, " " );
			if ( query.size() != 3 )
				throw BadRequest( "Bad request" );
			_method = *query.begin();
			_uri = *++query.begin();
			_protocol = *query.rbegin();
		}
		else
		{
			if ( s.size() > 2 && !body )
				extractHeader( _headers, s );
			else if ( s.size() <= 2 && !body )
				body = true;
			else
				_headers[ "Body" ] += s;
		}
		n++;
	}
	if ( body && content.size() )
		_headers[ "Body" ] += content;
	for ( std::map< std::string, std::string >::iterator it = _headers.begin(); it != _headers.end(); it++ )
		std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
}

Request::Request( const Request &other )
	: _method( other._method ), _uri( other._uri ), _protocol( other._protocol ), _host( other._host ),
	  _headers( other._headers )
{
}

Request &Request::operator=( const Request &other )
{
	( void )other;

	_headers = other._headers;
	_host = other._host;
	_method = other._method;
	_protocol = other._protocol;
	_uri = other._uri;
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