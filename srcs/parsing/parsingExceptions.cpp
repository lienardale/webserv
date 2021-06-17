/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingExceptions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 15:50:31 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/15 16:03:03 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/****************************************************************************************
 *			ParsingException
 ****************************************************************************************/

ParsingException::ParsingException( const std::string msg ) : _msg( msg )
{
}

ParsingException::~ParsingException() throw()
{
}

const char *ParsingException::what( void ) const throw()
{
	return _msg.c_str();
}

/****************************************************************************************
 *			SyntaxError
 ****************************************************************************************/
SyntaxError::SyntaxError( const std::string msg ) : ParsingException( msg ), _msg( "SyntaxError: " + msg )
{
}

SyntaxError::~SyntaxError() throw()
{
}

const char *SyntaxError::what() const throw()
{
	return _msg.c_str();
}

/****************************************************************************************
 *			ValueError
 ****************************************************************************************/
ValueError::ValueError( const std::string msg ) : ParsingException( msg ), _msg( "ValueError: " + msg )
{
}

ValueError::~ValueError() throw()
{
}

const char *ValueError::what() const throw()
{
	return _msg.c_str();
}
