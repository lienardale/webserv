/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:42:33 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/11 16:59:00 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
		if ( key == "listen" )
			handleListen( serverData, value );
		else if ( key == "server_name" )
			handleServerName( serverData, value );
		else if ( key == "root" )
			handleRoot( serverData, value );
		else if ( key == "autoindex" )
			handleAutoindex( serverData, value );
		else if ( key == "client_max_body_size" )
			handleClientMaxBody( serverData, value );
		else if ( key == "index" )
			handleIndex( serverData, value );
		else if ( key == "error_page" )
			handleErrorPage( serverData, value );
		else
			throw Parser::ParserException( "Key " + key + " is not valid" );

		// std::cout << "key: " << key << " -- value: " << value << std::endl;
		// std::cout << "listen: " << serverData.listen << std::endl;
		// std::cout << "server_name: " << serverData.server_name.front() << std::endl;
		// std::cout << "root: " << serverData.root << std::endl;
		// std::cout << "autoindex: " << serverData.autoindex << std::endl;
		// std::cout << "client_max_body_size: " << serverData.client_max_body_size << std::endl;
		// std::cout << "index: " << serverData.index.front() << std::endl;
		// for ( std::map< int, std::string >::iterator it = serverData.error_page.begin();
		//	  it != serverData.error_page.end(); it++ )
		//	std::cout << "Error_page key: " << it->first << " Error_page value: " << it->second << std::endl;

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
