/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingData.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 14:37:58 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/09 18:29:13 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGDATA_HPP
#define PARSINGDATA_HPP
#include <ctime>
#include <iostream>
#include <list>
#include <map>

typedef enum e_autoindex
{
	ON,
	OFF
} t_autoindex;

typedef struct s_locationData
{
	std::string path;
	std::string fastcgi_pass;
	std::list< std::string > index;
	std::map< std::string, std::string > fastcgi_param;
	t_autoindex autoindex;

} t_locationData;

typedef struct s_serverData
{
	int listen;
	std::string root;
	t_autoindex autoindex;
	int client_max_body_size;
	std::list< std::string > index;
	std::list< std::string > server_name;
	std::map< int, std::string > error_page;
	std::map< std::string, std::string > fastcgi_param;
	std::list< t_locationData > locations;

} t_serverData;

#endif