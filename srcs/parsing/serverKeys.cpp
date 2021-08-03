/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverKeys.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:47:24 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/02 19:19:35 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/dataStructure.hpp"
#include "parsing/parsingExceptions.hpp"
#include "parsing/utils.hpp"
#include "webserv.hpp"
#include <algorithm>
#include <cstddef>
#include <list>

static void handleListen(t_serverData &server, const std::string value) throw(ParsingException)
{
    if (server.listen != -1)
        throw ValueError("Duplicate key: listen");
    std::size_t found = value.find(":");
    if (found != std::string::npos)
    {
        server.addr_ip = value.substr(0, found);
        server.listen = strToInt(value.substr(found + 1, value.size() - found));
    }
    else
    {
        server.listen = strToInt(value);
        server.addr_ip = "0.0.0.0";
    }
}

static void handleServerName(t_serverData &server, const std::string value) throw(ParsingException)
{
    if (server.server_name.size())
        throw ValueError("Duplicate key: server_name");
    parseStringList< std::list< std::string > & >(server.server_name, value);
    server.server_name.sort();
}

static void handleAutoindex(t_serverData &server, const std::string value) throw(ParsingException)
{
    std::string error_msg = "Autoindex value should be 'on' or 'off' not " + value;

    if (value != "on" && value != "off")
        throw ValueError(error_msg);
    server.autoindex = value == "on";
}

static void handleRoot(t_serverData &server, const std::string value) throw(ParsingException)
{
    if (server.root.size())
        throw ValueError("Duplicate key: root");
    if (value[value.size() - 1] != '/')
        server.root = value + "/";
    else
        server.root = value;
}

static void handleClientMaxBody(t_serverData &server, const std::string value) throw(ParsingException)
{
    server.client_max_body_size = strToInt(value);
}

static void handleIndex(t_serverData &server, const std::string value) throw(ParsingException)
{
    if (server.index.size())
        throw ValueError("Duplicate key: index");
    parseStringList< std::list< std::string > & >(server.index, value);
}

static void fillErrorPage(t_serverData &server, const std::string value) throw(ParsingException)
{
    std::string::const_iterator begin = value.begin();
    std::string::const_iterator end = value.end();
    std::string key, _value;

    if (begin != end)
    {
        key = extract< std::string::const_iterator & >("\"\"", begin, end);
        if (server.error_page.find(strToInt(key)) != server.error_page.end())
            throw ValueError("Duplicate key: " + key);
        if (*begin == ':')
        {
            _value = extract< std::string::const_iterator & >("\"\"", ++begin, end);
            server.error_page[strToInt(key)] = _value;
            if (*begin == ',' && *(begin + 1) != '"')
            {
                std::string error_msg = "Expected value \" -- Actual value ";
                error_msg.push_back(*(begin + 1));
                throw SyntaxError(error_msg);
            }
            else if (*begin == ',')
                fillErrorPage(server, (++begin).base());
        }
    }
}

static void handleErrorPage(t_serverData &server, const std::string value) throw(ParsingException)
{
    if (server.error_page.size())
        throw ValueError("Duplicate key: error_page");
    fillErrorPage(server, value);
}

static bool compPath(t_locationData &first, t_locationData &second)
{
    return first.path <= second.path;
}

static void handleLocation(t_serverData &server, const std::string value) throw(std::exception)
{
    if (server.locations.size())
        throw ValueError("Duplicate key: location");
    server.locations = parseStructList< t_locationData, castLocation >(castLocationMap(), value);
    server.locations.sort(compPath);
}

static std::pair< std::string, castServer > KEY_FUNCTIONS[] = {
    std::make_pair("listen", handleListen),
    std::make_pair("server_name", handleServerName),
    std::make_pair("error_page", handleErrorPage),
    std::make_pair("client_max_body_size", handleClientMaxBody),
    std::make_pair("root", handleRoot),
    std::make_pair("autoindex", handleAutoindex),
    std::make_pair("index", handleIndex),
    std::make_pair("location", handleLocation),
};

std::map< std::string, castServer > castServerMap(void)
{
    return std::map< std::string, castServer >(KEY_FUNCTIONS,
                                               KEY_FUNCTIONS + sizeof(KEY_FUNCTIONS) / sizeof(*KEY_FUNCTIONS));
}
