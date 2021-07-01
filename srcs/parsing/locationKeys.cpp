/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationKeys.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 11:26:29 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/01 17:03:13 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONKEYS_HPP
#define LOCATIONKEYS_HPP
#include "parsing/parsingExceptions.hpp"
#include "parsing/utils.hpp"
#include "webserv.hpp"
#include <map>
#include <utility>

static void handleMethods(t_locationData &location, const std::string value) throw(ParsingException)
{
    if (location.methods.size())
        throw ValueError("Duplicate key: methods");
    parseStringList< std::list< std::string > & >(location.methods, value);
}

static void handleIndex(t_locationData &location, const std::string value) throw(ParsingException)
{
    if (location.index.size())
        throw ValueError("Duplicate key: Index");
    parseStringList< std::list< std::string > & >(location.index, value);
}

static void handlePath(t_locationData &location, const std::string value) throw(ParsingException)
{
    if (location.path.size())
        throw ValueError("Duplicate key: path");
    location.path = value;
}

static void handleAutoindex(t_locationData &location, const std::string value) throw(ParsingException)
{
    std::string error_msg = "Value error: autoindex value should be 'on' or 'off' not " + value;

    if (value != "on" && value != "off")
        throw ValueError(error_msg);
    location.autoindex = value == "on";
}

static void fillFastCGI(t_locationData &location, const std::string value) throw(ParsingException)
{
    std::string::const_iterator begin = value.begin();
    std::string::const_iterator end = value.end();
    std::string key, _value;

    if (begin != end)
    {
        key = extract< std::string::const_iterator & >("\"\"", begin, end);
        if (location.fastcgi_param.find(key) != location.fastcgi_param.end())
            throw ValueError("Duplicate key: " + key);
        if (*begin == ':')
        {
            _value = extract< std::string::const_iterator & >("\"\"", ++begin, end);
            location.fastcgi_param[key] = _value;
            if (*begin == ',' && *(begin + 1) != '"')
            {
                std::string error_msg = "Expected value \" -- Actual value ";
                error_msg.push_back(*(begin + 1));
                throw SyntaxError(error_msg);
            }
            else if (*begin == ',')
                fillFastCGI(location, (++begin).base());
        }
    }
}

static void handleFastCGI(t_locationData &location, const std::string value) throw(ParsingException)
{
    if (location.fastcgi_param.size())
        throw ValueError("Duplicate key: fastcgi_param");
    fillFastCGI(location, value);
}

static std::pair< std::string, castLocation > KEY_FUNCTIONS[] = {
    std::make_pair("methods", handleMethods), std::make_pair("path", handlePath), std::make_pair("index", handleIndex),
    std::make_pair("autoindex", handleAutoindex), std::make_pair("fastcgi_param", handleFastCGI)};

std::map< std::string, castLocation > castLocationMap(void)
{
    return std::map< std::string, castLocation >(KEY_FUNCTIONS,
                                                 KEY_FUNCTIONS + sizeof(KEY_FUNCTIONS) / sizeof(*KEY_FUNCTIONS));
}

#endif
