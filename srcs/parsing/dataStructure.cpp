/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dataStructure.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 16:14:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/28 13:29:18 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::ostream &operator<<(std::ostream &os, const t_locationData &l)
{
    os << "{ " << std::endl << "\t\tfastcgi_param: " << l.fastcgi_param << std::endl;
    os << "root: " << l.root << std::endl;
    os << "\t\tindex: " << l.index << std::endl;
    os << "\t\tautoindex: " << l.autoindex << std::endl;
    os << "\t\tmethods: " << l.methods << std::endl;
    os << "\t\troot: " << l.root << std::endl;
    os << "\t\tpath: " << l.path << std::endl;
    os << "\t}";
    return os;
}

std::ostream &operator<<(std::ostream &os, const t_serverData &l)
{
    os << "{ " << std::endl << "\t listen: " << l.listen << std::endl;
    os << "\t addr_ip: " << l.addr_ip << std::endl;
    os << "\t server_name: " << l.server_name << std::endl;
    os << "\t root: " << l.root << std::endl;
    os << "\t index: " << l.index << std::endl;
    os << "\t error_page: " << l.error_page << std::endl;
    os << "\t client_max_body_size: " << l.client_max_body_size << std::endl;
    os << "\t autoindex: " << l.autoindex << std::endl;
    os << "\t location: " << l.locations << std::endl;
    os << "}";

    return os;
}
/************************************************************************************
 *			Constructeurs
 ***********************************************************************************/
t_serverData::s_serverData(void) : listen(-1), autoindex(false), client_max_body_size(1000)
{
}

t_locationData::s_locationData(void) : autoindex(false)
{
}
