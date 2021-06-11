/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:48:58 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/11 16:56:13 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP
#include <exception>
#include <iostream>
#include <list>

/****************************************************************************************
 *			Extraction des éléments
 ****************************************************************************************/
std::string extract( const std::string envel, std::string::iterator &begin,
					 std::string::iterator &end ) throw( std::exception );
std::string extract( const std::string envel, std::string::const_iterator &begin,
					 std::string::const_iterator &end ) throw( std::exception );
void extract_server( std::list< std::string > &servers, std::string::iterator &begin,
					 std::string::iterator &end ) throw( std::exception );

/****************************************************************************************
 *               		Conversion
 ***************************************************************************************/

int strToInt( const std::string &value ) throw( std::exception );

#endif
