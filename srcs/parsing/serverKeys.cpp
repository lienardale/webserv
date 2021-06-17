/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverKeys.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:47:24 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/16 15:24:34 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/dataStructure.hpp"
#include "parsing/parsingExceptions.hpp"
#include "webserv.hpp"

static void handleListen( t_serverData &server, const std::string value ) throw( ParsingException )
{
	server.listen = strToInt( value );
}

static void handleServerName( t_serverData &server, const std::string value ) throw( ParsingException )
{
	parseStringList< std::list< std::string > & >( server.server_name, value );
}

static void handleAutoindex( t_serverData &server, const std::string value ) throw( ParsingException )
{
	std::string error_msg = "Autoindex value should be 'on' or 'off' not " + value;

	if ( value != "on" && value != "off" )
		throw ValueError( error_msg );
	server.autoindex = value == "on";
}

static void handleRoot( t_serverData &server, const std::string value ) throw( ParsingException )
{
	server.root = value;
}

static void handleClientMaxBody( t_serverData &server, const std::string value ) throw( ParsingException )
{
	server.client_max_body_size = strToInt( value );
}

static void handleIndex( t_serverData &server, const std::string value ) throw( ParsingException )
{
	parseStringList< std::list< std::string > & >( server.index, value );
}

static void handleErrorPage( t_serverData &server, const std::string value ) throw( ParsingException )
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
			server.error_page[ strToInt( key ) ] = _value;
			if ( *begin == ',' && *( begin + 1 ) != '"' )
			{
				std::string error_msg = "Expected value \" -- Actual value ";
				error_msg.push_back( *( begin + 1 ) );
				throw SyntaxError( error_msg );
			}
			else if ( *begin == ',' )
				handleErrorPage( server, ( ++begin ).base() );
		}
	}
}

static void handleLocation( t_serverData &server, const std::string value ) throw( std::exception )
{
	server.locations = parseStructList< t_locationData, castLocation >( castLocationMap(), value );
}

static std::pair< std::string, castServer > KEY_FUNCTIONS[] = {
	std::make_pair( "listen", handleListen ),
	std::make_pair( "server_name", handleServerName ),
	std::make_pair( "error_page", handleErrorPage ),
	std::make_pair( "client_max_body_size", handleClientMaxBody ),
	std::make_pair( "root", handleRoot ),
	std::make_pair( "autoindex", handleAutoindex ),
	std::make_pair( "index", handleIndex ),
	std::make_pair( "location", handleLocation ),
};

std::map< std::string, castServer > castServerMap( void )
{
	return std::map< std::string, castServer >( KEY_FUNCTIONS,
												KEY_FUNCTIONS + sizeof( KEY_FUNCTIONS ) / sizeof( *KEY_FUNCTIONS ) );
}
