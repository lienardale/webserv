/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:38:51 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/11 16:46:19 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/****************************************************************************************
 *				Extraction
 ****************************************************************************************/
std::string extract( const std::string envel, std::string::iterator &begin,
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

std::string extract( const std::string envel, std::string::const_iterator &begin,
					 std::string::const_iterator &end ) throw( std::exception )
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

void extract_server( std::list< std::string > &servers, std::string::iterator &begin,
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

int strToInt( const std::string &value ) throw( std::exception )
{
	std::string error_msg = value + " is not a number";
	for ( std::string::const_iterator it = value.begin(); it != value.end(); it++ )
		if ( !std::isdigit( *it ) )
			throw Parser::ParserException( error_msg );
	return atoi( value.c_str() );
}
