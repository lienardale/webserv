/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationKeys.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 11:26:29 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/16 15:25:38 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONKEYS_HPP
#define LOCATIONKEYS_HPP
#include "parsing/parsingExceptions.hpp"
#include "parsing/utils.hpp"
#include "webserv.hpp"
#include <map>
#include <utility>

static void handleMethods( t_locationData &location, const std::string value ) throw( ParsingException )
{
	parseStringList< std::list< std::string > & >( location.methods, value );
}

static void handleIndex( t_locationData &location, const std::string value ) throw( ParsingException )
{
	parseStringList< std::list< std::string > & >( location.index, value );
}

static void handleRoot( t_locationData &location, const std::string value ) throw( ParsingException )
{
	location.root = value;
}

static void handlePath( t_locationData &location, const std::string value ) throw( ParsingException )
{
	location.path = value;
}

static void handleAutoindex( t_locationData &location, const std::string value ) throw( ParsingException )
{
	std::string error_msg = "Value error: autoindex value should be 'on' or 'off' not " + value;

	if ( value != "on" && value != "off" )
		throw ValueError( error_msg );
	location.autoindex = value == "on";
}

static void handleFastCGI( t_locationData &location, const std::string value ) throw( ParsingException )
{
	std::string::const_iterator begin = value.begin();
	std::string::const_iterator end = value.end();
	std::string key, _value;

	if ( begin != end )
	{
		key = extract< std::string::const_iterator & >( "\"\"", begin, end );
		if ( *begin == ':' )
		{
			_value = extract< std::string::const_iterator & >( "\"\"", ++begin, end );
			location.fastcgi_param[ key ] = _value;
			if ( *begin == ',' && *( begin + 1 ) != '"' )
			{
				std::string error_msg = "Expected value \" -- Actual value ";
				error_msg.push_back( *( begin + 1 ) );
				throw SyntaxError( error_msg );
			}
			else if ( *begin == ',' )
				handleFastCGI( location, ( ++begin ).base() );
		}
	}
}

static std::pair< std::string, castLocation > KEY_FUNCTIONS[] = {
	std::make_pair( "methods", handleMethods ),		std::make_pair( "path", handlePath ),
	std::make_pair( "index", handleIndex ),			std::make_pair( "root", handleRoot ),
	std::make_pair( "autoindex", handleAutoindex ), std::make_pair( "fastcgi_param", handleFastCGI ) };

std::map< std::string, castLocation > castLocationMap( void )
{
	return std::map< std::string, castLocation >( KEY_FUNCTIONS,
												  KEY_FUNCTIONS + sizeof( KEY_FUNCTIONS ) / sizeof( *KEY_FUNCTIONS ) );
}

#endif
