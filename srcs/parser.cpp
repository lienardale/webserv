/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:42:33 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/10 18:59:47 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <cctype>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

/****************************************************************************************
 *               	Nettoyage et lecture de fichier
 ***************************************************************************************/
static std::string _cleanConfig( const std::string &config )
{
	char quote = 0;
	std::string result( "" );

	for ( std::string::const_iterator it = config.begin(); it != config.end(); it++ )
	{
		if ( !std::isspace( *it ) || ( std::isspace( *it ) && quote ) )
			result.push_back( *it );
		if ( *it == '"' )
			quote = quote == *it ? 0 : *it;
	}
	return result;
}

static std::string _readFile( std::fstream &fs )
{
	std::string ret( "" );
	for ( std::string line; std::getline( fs, line ); )
		ret += line;

	return _cleanConfig( ret );
}

/****************************************************************************************
 *               	Extraction d'informations
 ***************************************************************************************/

static std::string extract( const std::string envel, std::string::iterator &begin,
							std::string::iterator &end ) throw( std::exception )
{
	if ( envel.size() != 2 || *begin != *envel.begin() )
		throw Parser::ParserException( "Cannot extract data" );

	std::cout << std::endl << "Before: " << envel << " " << begin.base() << std::endl;
	std::string ret;
	int first_count = 1;
	int end_count = 0;
	while ( ++begin != end && ( ( first_count != end_count && *envel.begin() != *++envel.begin() ) ||
								( *envel.begin() == *++envel.begin() && first_count != 2 ) ) )
	{
		if ( *begin == *envel.begin() )
			first_count++;
		if ( *begin == *++envel.begin() )
			end_count++;
		if ( ( ( first_count != end_count && *envel.begin() != *++envel.begin() ) ||
			   ( *envel.begin() == *++envel.begin() && first_count != 2 ) ) )
			ret.push_back( *begin );
	}
	std::cout << std::endl << "After: " << envel << " " << ret << " " << first_count << " " << end_count << std::endl;
	if ( ( ( first_count != end_count && *envel.begin() != *++envel.begin() ) ||
		   ( *envel.begin() == *++envel.begin() && first_count != 2 ) ) )
		throw Parser::ParserException( "Cannot extract data" );
	return ret;
}

static void extract_server( std::list< std::string > &servers, std::string::iterator &begin,
							std::string::iterator &end ) throw( std::exception )
{
	if ( begin != end && *begin == '{' )
	{
		std::string ret = extract( "{}", begin, end );
		if ( ret.size() )
		{
			servers.push_back( ret );
			if ( begin != end )
				extract_server( servers, ++begin, end );
		}
	}
}
/****************************************************************************************
 *               		Conversion
 ***************************************************************************************/

/*static int strToInt( const std::string &value ) throw( std::exception )
{
	std::string error_msg = value + " is not a number";
	for ( std::string::const_iterator it = value.begin(); it != value.end(); it++ )
		if ( !std::isdigit( *it ) )
			throw Parser::ParserException( error_msg );
	return atoi( value.c_str() );
}

static std::string handleString( const std::string &key, const std::string &value ) throw( std::exception )
{
	std::string error_msg = "Value error: values for key \"" + key + "\" must be strings";
	if ( *value.begin() != '\"' || ( value.size() > 1 && *--value.end() != '\"' ) )
		throw Parser::ParserException( error_msg );

	std::string::const_iterator val_begin = value.begin();
	std::string::const_iterator val_end = value.end();
	return extract( "\"\"", val_begin, val_end );
}*/

/*static void handleListen( t_serverData &server, const std::string value ) throw( std::exception )
{
	server.listen = strToInt( value );
}

static void handleServerName( t_serverData &server, const std::string value ) throw( std::exception )
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

static void handleErrorPage( t_serverData &server, const std::string value ) throw( std::exception )
{
}

typedef void ( *castFunction )( t_serverData &, const std::string );

std::pair< std::string, castFunction > KEY_FUNCTIONS[] = {
	std::make_pair( "listen", handleListen ),
	std::make_pair( "server_name", handleServerName ),
	std::make_pair( "error_page", handleErrorPage ),
	std::make_pair( "client_max_body_size", handleClientMaxBody ),
	std::make_pair( "root", handleRoot ),
	std::make_pair( "autoindex", handleAutoindex ),
	std::make_pair( "index", handleIndex ),
	std::make_pair( "location", handleLocation ),
};
*/
static bool isListKey( const std::string &key )
{
	return ( key == "server_name" || key == "location" || key == "error_page" || key == "index" );
}

static void fillServerData( t_serverData &serverData, std::string::iterator begin,
							std::string::iterator end ) throw( std::exception )
{
	std::string key, value;
	if ( begin != end )
	{
		if ( *begin == ',' )
			++begin;
		key = extract( "\"\"", begin, end );
		if ( isListKey( key ) )
			value = extract( "[]", ++begin, end );
		else
			value = extract( ":,", begin, end );
		std::cout << "key: " << key << " -- value: " << value << std::endl;
		fillServerData( serverData, begin, end );
	}
}

static t_serverData parseServer( std::string::iterator begin, std::string::iterator end ) throw( std::exception )
{
	t_serverData data;

	if ( begin == end )
		throw Parser::ParserException( "Wrong server configuration" );
	fillServerData( data, begin, end );
	return data;
}

/******************************************************************************
 *               Construtors
 ******************************************************************************/
Parser::Parser( std::fstream &fs ) : _content( _readFile( fs ) )
{
	std::string::iterator begin = _content.begin();
	std::string::iterator end = _content.end();
	_content = extract( "[]", begin, end );
	parse();
}

Parser::Parser( const Parser &other )
{
	*this = other;
}

Parser &Parser::operator=( const Parser &other )
{
	_content = other._content;
	return *this;
}

Parser::~Parser( void )
{
}

/******************************************************************************
 *              Member functions
 ******************************************************************************/

std::list< t_serverData > Parser::parse( void ) throw( std::exception )
{
	std::list< t_serverData > ret;
	std::string::iterator begin = _content.begin();
	std::string::iterator end = _content.end();
	std::list< std::string > servers;

	extract_server( servers, begin, end );

	if ( servers.size() == 0 )
		throw Parser::ParserException( "No valid server in config file" );

	for ( std::list< std::string >::iterator it = servers.begin(); it != servers.end(); it++ )
		parseServer( it->begin(), it->end() );

	return ret;
}
