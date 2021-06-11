/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverKeys.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:47:24 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/11 16:57:12 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void handleListen( t_serverData &server, const std::string value ) throw( std::exception )
{
	server.listen = strToInt( value );
}

void handleServerName( t_serverData &server, const std::string value ) throw( std::exception )
{
	std::string tmp;

	for ( std::string::const_iterator it = value.begin(); it != value.end(); it++ )
	{
		if ( *it != ',' )
			tmp.push_back( *it );
		else
		{
			server.server_name.push_back( tmp );
			tmp.clear();
		}
	}
}

void handleAutoindex( t_serverData &server, const std::string value ) throw( std::exception )
{
	std::string error_msg = "Value error: autoindex value should be 'on' or 'off' not " + value;

	std::string::const_iterator begin = value.begin();
	std::string::const_iterator end = value.end();
	std::string clean_value = extract( "\"\"", begin, end );

	if ( clean_value != "on" && clean_value != "off" )
		throw Parser::ParserException( error_msg );
	server.autoindex = value == "on";
}

void handleRoot( t_serverData &server, const std::string value ) throw( std::exception )
{
	std::string::const_iterator begin = value.begin();
	std::string::const_iterator end = value.end();
	server.root = extract( "\"\"", begin, end );
}

void handleClientMaxBody( t_serverData &server, const std::string value ) throw( std::exception )
{
	// std::string error_msg = "Value error: client_max_body_size must a number. Your actual value -> " + value;
	server.client_max_body_size = strToInt( value );
}

void handleIndex( t_serverData &server, const std::string value ) throw( std::exception )
{
	std::string tmp;

	for ( std::string::const_iterator it = value.begin(); it != value.end(); it++ )
	{
		if ( *it != ',' )
			tmp.push_back( *it );
		else
		{
			server.index.push_back( tmp );
			tmp.clear();
		}
	}
}

void handleErrorPage( t_serverData &server, const std::string value ) throw( std::exception )
{
	std::string page_value, page;
	std::string::const_iterator begin = value.begin();
	std::string::const_iterator end = value.end();

	if ( *begin == '[' )
		handleErrorPage( server, extract( "[]", begin, end ) );
	else if ( *begin == '{' )
	{
		page = extract( "{}", begin, end );
		std::string::iterator page_begin = page.begin();
		std::string::iterator page_end = page.end();
		int key = strToInt( extract( "\"\"", page_begin, page_end ) );
		if ( *page_begin == ':' )
		{
			++page_begin;
			page_value = extract( "\"\"", page_begin, page_end );
			server.error_page[ key ] = page_value;
			if ( *begin == ',' )
				handleErrorPage( server, std::string( begin.base() ) );
		}
		else
			throw Parser::ParserException( "Wrong error page format" );
	}
}

/*void handleLocation( t_serverData &server, const std::string value ) throw( std::exception )
{
	std::string page_value, page;
	std::string::const_iterator begin = value.begin();
	std::string::const_iterator end = value.end();
}*/

/*typedef void ( *castFunction )( t_serverData &, const std::string );

std::pair< std::string, castFunction > KEY_FUNCTIONS[] = {
	std::make_pair( "listen", handleListen ),
	std::make_pair( "server_name", handleServerName ),
	std::make_pair( "error_page", handleErrorPage ),
	std::make_pair( "client_max_body_size", handleClientMaxBody ),
	std::make_pair( "root", handleRoot ),
	std::make_pair( "autoindex", handleAutoindex ),
	std::make_pair( "index", handleIndex ),
	std::make_pair( "location", handleLocation ),
};*/
