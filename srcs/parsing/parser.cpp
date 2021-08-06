/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:42:33 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/06 11:03:13 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/dataStructure.hpp"
#include "parsing/parsingExceptions.hpp"
#include "webserv.hpp"
#include <algorithm>
#include <list>
#include <map>

/****************************************************************************************
 *               	Nettoyage et lecture de fichier
 ***************************************************************************************/
static std::string _cleanConfig(const std::string &config)
{
    char quote = 0;
    std::string result("");

    for (std::string::const_iterator it = config.begin(); it != config.end(); it++)
    {
        if (!std::isspace(*it) || (std::isspace(*it) && quote))
            result.push_back(*it);
        if (*it == '"')
            quote = quote == *it ? 0 : *it;
    }
    return result;
}

static std::string _readFile(std::fstream &fs)
{
    std::string ret("");
    for (std::string line; std::getline(fs, line);)
        ret += line;

    return _cleanConfig(ret);
}

/******************************************************************************
 *               Construtors
 ******************************************************************************/
Parser::Parser(std::fstream &fs) : _content(_readFile(fs))
{
    std::string::iterator begin = _content.begin();
    std::string::iterator end = _content.end();
    _content = extract("[]", begin, end);
}

Parser::Parser(const Parser &other)
{
    *this = other;
}

Parser &Parser::operator=(const Parser &other)
{
    _content = other._content;
    return *this;
}

Parser::~Parser(void)
{
}

/******************************************************************************
 *              Member functions
 ******************************************************************************/

static bool _checkLocationDup(std::list< t_locationData > &locations)
{
    std::map< std::string, bool > paths;
    for (std::list< t_locationData >::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (paths.count(it->path))
            return false;
        paths[it->path] = true;
    }
    return true;
}

std::list< t_serverData > Parser::parse(void) throw(ParsingException)
{
    std::list< t_serverData > res = parseStructList< t_serverData >(castServerMap(), _content);
    for (std::list< t_serverData >::iterator it = res.begin(); it != res.end(); it++)
    {
        if (!_checkLocationDup(it->locations))
            throw ParsingException("Duplicate path in locations");
    }
    return res;
}
