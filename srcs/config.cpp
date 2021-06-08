/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 14:31:53 by alienard          #+#    #+#             */
/*   Updated: 2021/06/08 16:19:34 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

/******************************************************************************
 *			Constructeurs/Destructeurs
 *****************************************************************************/

config::config()
{
	std::fstream fs;
	// std::map<std::string, std::string>	config;
	// fs.open("config/nginx.conf",std::fstream::in);
	fs.open("webserv.conf",std::fstream::in);
	if (!fs.is_open()){
		std::cerr << "Error : " << strerror(errno) << std::endl;
		throw ConfigFileException();
	}
	for (std::string line; std::getline(fs, line); ) {
		std::cout << "line:" << line << std::endl;
	}
	fs.close();
}

/*
 *	Constructeur par copie
 */
config::config(const config &other)
{
	*this = other;
}

/*
 *	Construction par assignation
 */
config	&config::operator=(const config &other)
{
	this->server = other.getServer();
	this->port = other.getPort();
	return (*this);
}

config::~config(void)
{
}

std::string	config::getServer(void) const{
	return this->server;
}

int	config::getPort(void) const{
	return this->port;
}

/*
int config::ft_print_config(void)
{
	std::fstream fs;
	// std::map<std::string, std::string>	config;
	// fs.open("config/nginx.conf",std::fstream::in);
	fs.open("webserv.conf",std::fstream::in);
	if (!fs.is_open()){
		std::cerr << "Error : " << strerror(errno) << std::endl;
		return(1);
	}
	for (std::string line; std::getline(fs, line); ) {
		std::cout << "line:" << line << std::endl;
	}
	fs.close();
	return 0;
}
*/
