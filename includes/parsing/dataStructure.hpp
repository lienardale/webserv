/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dataStructure.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 14:37:58 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/01 12:39:17 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP
#include <ctime>
#include <iostream>
#include <list>
#include <map>

typedef struct s_locationData
{
    std::string path;
    std::list<std::string> index;
    std::map<std::string, std::string> fastcgi_param;
    bool autoindex;
    std::list<std::string> methods;
    s_locationData(void);

} t_locationData;

typedef struct s_serverData
{
    int listen;
    std::string addr_ip;
    std::string root;
    bool autoindex;
    int client_max_body_size;
    std::list<std::string> index;
    std::list<std::string> server_name;
    std::map<int, std::string> error_page;
    std::list<t_locationData> locations;
    s_serverData(void);
} t_serverData;

/****************************************************************************************
 *				Operator overloading
 ****************************************************************************************/
std::ostream &operator<<(std::ostream &os, const t_locationData &l);
std::ostream &operator<<(std::ostream &os, const t_serverData &l);

/****************************************************************************************
 *				Operator overloading (template)
 ****************************************************************************************/
template <typename T> std::ostream &operator<<(std::ostream &os, const std::list<T> &l)
{
    os << "[ ";

    for (typename std::list<T>::const_iterator it = l.begin(); it != l.end(); it++)
        os << *it << " ";

    os << "]";
    return os;
}

template <typename key, typename value> std::ostream &operator<<(std::ostream &os, const std::map<key, value> &l)
{
    os << "{ " << std::endl << "\t\t";

    for (typename std::map<key, value>::const_iterator it = l.begin(); it != l.end(); it++)
        os << "( " << it->first << ", " << it->second << " ) ";

    os << std::endl << "\t}";
    return os;
}

#endif
