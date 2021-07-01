/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:38:51 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/01 17:03:24 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parsingExceptions.hpp"
#include "webserv.hpp"

/****************************************************************************************
 *               		Conversion
 ***************************************************************************************/

int strToInt(const std::string &value) throw(ValueError)
{
    std::string error_msg = value + " is not a number";
    for (std::string::const_iterator it = value.begin(); it != value.end(); it++)
        if (!std::isdigit(*it))
            throw ValueError(error_msg);
    return atoi(value.c_str());
}
