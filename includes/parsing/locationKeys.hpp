/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationKeys.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:36:33 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/11 16:36:36 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#include "parsingData.hpp"
#include <exception>
#include <fstream>

class Parser
{
  public:
	Parser( std::fstream &fs );
	Parser( const Parser &other );
	Parser &operator=( const Parser &other );
	~Parser( void );

	std::list< t_serverData > parse( void ) throw( std::exception );

	// Member exception
	class ParserException : public std::exception
	{
	  public:
		const char *what( void ) const throw()
		{
			return _msg.c_str();
		}
		ParserException( std::string msg ) : _msg( msg )
		{
		}
		virtual ~ParserException() throw()
		{
		}

	  private:
		std::string _msg;
	};

  private:
	std::string _content;
};

#endif
