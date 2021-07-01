/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:48:58 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/16 15:13:49 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP
#include "dataStructure.hpp"
#include "parsingExceptions.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>

/****************************************************************************************
 *			Extraction des éléments
 ****************************************************************************************/

template <typename iterator>
std::string extract(const std::string envel, iterator begin, iterator end) throw(ParsingException)
{
    if (envel.size() != 2)
        throw ParsingException("Wrong usage of extract function. Envel parameter must have a size of 2");
    if (*begin != *envel.begin())
    {
        std::string error_msg = "Value expected '";
        error_msg.push_back(*envel.begin());
        error_msg += "' -- Actual value: '";
        error_msg.push_back(*begin);
        error_msg += "'";
        throw ValueError(error_msg);
    }

    // std::cout << std::endl << "Before: " << envel << " " << begin.base() << std::endl;
    std::string ret;
    int first_count = 1;
    int end_count = 0;
    while (++begin != end && ((first_count != end_count && *envel.begin() != *++envel.begin()) ||
                              (*envel.begin() == *++envel.begin() && first_count != 2)))
    {
        if (*begin == *envel.begin())
            first_count++;
        if (*begin == *++envel.begin())
            end_count++;
        if (((first_count != end_count && *envel.begin() != *++envel.begin()) ||
             (*envel.begin() == *++envel.begin() && first_count != 2)))
            ret.push_back(*begin);
    }
    // std::cout << std::endl << "After: " << envel << " " << ret << " " << first_count << " " << end_count <<
    // std::endl;
    if (((first_count != end_count && *envel.begin() != *++envel.begin()) ||
         (*envel.begin() == *++envel.begin() && first_count != 2)))
    {
        std::string error_msg = begin.base();
        error_msg += " is not terminated by ";
        error_msg.push_back(*--envel.end());
        throw SyntaxError(error_msg);
    }
    return ret;
}

template <typename iterator>
void extractStructList(std::list<std::string> &store, iterator begin, iterator end) throw(ParsingException)
{
    if (begin != end)
    {
        std::string ret = extract<iterator>("{}", begin, end);
        if (ret.size())
        {
            store.push_back(ret);
            if (*begin == ',' && *(begin + 1) != '{')
            {
                std::string error_msg = "expected value: { -- Actual value ";
                error_msg.push_back(*(begin + 1));
                throw SyntaxError(error_msg);
            }
            else if (*begin == ',')
                extractStructList<iterator>(store, ++begin, end);
        }
    }
}

/****************************************************************************************
 *			Dynamique principale du parser
 ****************************************************************************************/

template <typename dataStore> void parseStringList(dataStore &store, const std::string value) throw(ParsingException)
{
    std::string::const_iterator begin = value.begin();
    std::string::const_iterator end = value.end();

    if (begin != end)
    {
        store.push_back(extract<std::string::const_iterator &>("\"\"", begin, end));
        if (*begin == ',' && *(begin + 1) != '"')
        {
            std::string error_msg = "expected value: \" -- Actual value ";
            error_msg.push_back(*(begin + 1));
            throw SyntaxError(error_msg);
        }
        else if (*begin == ',')
            parseStringList<dataStore>(store, (++begin).base());
    }
}

template <typename dataStore, typename conversion>
void parseStructFields(dataStore &store, const std::map<std::string, conversion> keyMap, std::string::iterator begin,
                       std::string::iterator end) throw(ParsingException)
{
    std::string key, value;
    typename std::map<std::string, conversion>::const_iterator found;

    if (begin != end)
    {
        if (*begin == ',' && *(begin + 1) != '"' && *(begin + 1) != '{')
        {
            std::string error_msg = "expected value: \" or { -- Actual value ";
            error_msg.push_back(*(begin + 1));
            throw SyntaxError(error_msg);
        }
        else if (*begin == ',')
            parseStructFields<dataStore, conversion>(store, keyMap, ++begin, end);
        else
        {
            key = extract<std::string::iterator &>("\"\"", begin, end);
            found = keyMap.find(key);
            if (found == keyMap.end())
                throw ValueError("Key " + key + " is not valid");
            if (*(begin + 1) == '{')
                value = extract<std::string::iterator &>("{}", ++begin, end);
            else if (*(begin + 1) == '[')
                value = extract<std::string::iterator &>("[]", ++begin, end);
            else if (*(begin + 1) == '"')
                value = extract<std::string::iterator &>("\"\"", ++begin, end);
            else if (*begin == ':')
                value = extract<std::string::iterator &>(":,", begin, end);
            else
                throw SyntaxError("There must be a ':' after the key " + key);
            found->second(store, value);
            parseStructFields<dataStore, conversion>(store, keyMap, begin, end);
        }
    }
}

template <typename dataStore, typename conversion>
dataStore parseStruct(std::map<std::string, conversion> conv, std::string::iterator begin,
                      std::string::iterator end) throw(ParsingException)
{
    dataStore data;

    if (begin == end)
        throw SyntaxError("Unexpected EOF");
    parseStructFields<dataStore &, conversion>(data, conv, begin, end);
    return data;
}

template <typename dataStore, typename conversion>
std::list<dataStore> parseStructList(std::map<std::string, conversion> conv,
                                     const std::string value) throw(ParsingException)
{
    std::list<dataStore> ret;
    std::list<std::string> tmp;
    std::string::const_iterator begin = value.begin();
    std::string::const_iterator end = value.end();

    extractStructList<std::string::const_iterator &>(tmp, begin, end);

    if (tmp.size() == 0)
        throw ValueError("Empty list of struct");

    for (std::list<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
        ret.push_back(parseStruct<dataStore, conversion>(conv, it->begin(), it->end()));

    return ret;
}
/****************************************************************************************
 *               		Conversion
 ***************************************************************************************/

typedef void (*castLocation)(t_locationData &, const std::string);
typedef void (*castServer)(t_serverData &, const std::string);

int strToInt(const std::string &value) throw(ValueError);
std::map<std::string, castLocation> castLocationMap(void);
std::map<std::string, castServer> castServerMap(void);

#endif
