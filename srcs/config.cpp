/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 14:31:53 by alienard          #+#    #+#             */
/*   Updated: 2021/06/18 11:48:24 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parsingExceptions.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <exception>

/******************************************************************************
 *			Constructeurs/Destructeurs
 *****************************************************************************/

config::config()
{
	std::fstream fs;
	fs.open( "config.json", std::fstream::in );

	if ( !fs.is_open() )
	{
		std::cerr << "Error : " << strerror( errno ) << std::endl;
		throw ConfigFileException();
	}
	try
	{
		_content = Parser( fs ).parse();
		config_check(&_content);
		// std::cout << _content << std::endl;
		std::cout << GREEN << "Parsing [OK]" << std::endl;
		std::cout << WHITE << std::endl;
	}
	catch ( std::exception &e )
	{
		std::cerr << RED << e.what() << std::endl;
		std::cerr << "Parsing [KO]" << std::endl;
		exit( EXIT_FAILURE );
	}
	fs.close();
}

/*
 *	Constructeur avec path donné
 */

config::config(char *path)
{
	std::fstream fs;
	if (path)
		fs.open( path, std::fstream::in );
	else
		fs.open( "config.json", std::fstream::in );

	if ( !fs.is_open() )
	{
		std::cerr << "Error : " << strerror( errno ) << std::endl;
		throw ConfigFileException();
	}
	try
	{
		_content = Parser( fs ).parse();
		config_check(&_content);
		// std::cout << _content << std::endl;
		std::cout << GREEN << "Parsing [OK]" << std::endl;
		std::cout << WHITE << std::endl;
	}
	catch ( std::exception &e )
	{
		std::cerr << RED << e.what() << std::endl;
		std::cerr << "Parsing [KO]" << std::endl;
		exit( EXIT_FAILURE );
	}
	fs.close();
}

/*
 *	Constructeur par copie
 */
config::config( const config &other )
{
	*this = other;
}

/*
 *	Construction par assignation
 */
config &config::operator=( const config &other )
{
	this->server = other.getServer();
	this->port = other.getPort();
	return ( *this );
}

config::~config( void )
{
}

std::string config::getServer( void ) const
{
	return this->server;
}

int config::getPort( void ) const
{
	return this->port;
}

void	config::methods_check(std::string &method){
	// std::cout << method << std::endl;
	if (method.compare(SSTR("GET")) && method.compare(SSTR("POST")) && method.compare(SSTR("DELETE")))
		throw ValueError::ParsingException("incorrect method value : " + method + ", must be GET, POST, and/or DELETE");
	(void) method;
}

void	config::fastcgi_param_check( std::pair< const std::string, std::string > &fcgi){
	// fcgi.first
	// fcgi.second
	(void) fcgi;
}

int		config::lD_index_check(const char *dir, std::string &index){
	DIR *dirp;
	struct dirent *tmp;
	dirp = opendir(dir);
	while ((tmp = readdir(dirp)) != NULL) {
		if (strlen(tmp->d_name) /*d_namlen*/ == index.size() && strcmp(tmp->d_name, index.c_str()) == 0) {
			(void)closedir(dirp);
			return (1);
		}
	}
	return 0;
	(void) index;
}

void	config::locationData_check(t_locationData &lD){
	// lD.path -> not checking validity

	// lD.root
	DIR *dirp;
	if ((dirp = opendir(lD.root.c_str())) == NULL /*chdir(sD.root.c_str()) == -1*/)
		throw ValueError::ParsingException("incorrect root value : " + lD.root + ", must be existing dir");
	else
		closedir(dirp);

	// checking index
	int	check = 0;
	for ( std::list< std::string >::iterator it = lD.index.begin() ; it != lD.index.end() ; it++ ){
		check += config::lD_index_check(lD.root.c_str(), *it);
	}
	if (!check)
		throw ValueError::ParsingException("incorrect index value : no corresponding file found");

	// checking fast_cgi_param
	for ( std::map< std::string, std::string >::iterator it = lD.fastcgi_param.begin() ; it != lD.fastcgi_param.end() ; it++ ){
		// check if binary present ?
		config::fastcgi_param_check(*it);
	}

	// checking autoindex
	if (lD.autoindex != true && lD.autoindex != false)
		throw ValueError::ParsingException("incorrect autoindex");

	// checking methods
	for ( std::list< std::string >::iterator it = lD.methods.begin() ; it != lD.methods.end() ; it++ ){
		config::methods_check(*it);
	}
}

void	config::error_page_check(std::pair< const int, std::string > &error_page){
	// error_page.first 
	// error_page.second 
	std::fstream fs;
	fs.open ( error_page.second.c_str(), std::fstream::in );
	if (!fs.is_open())
		throw ValueError::ParsingException("incorrect error_page path : " + error_page.second + ", must be existing file");
	else
		fs.close();
}

int		config::sD_index_check(const char *dir, std::string &index){
	// std::cout << dir << index << std::endl;
	// (void) index;
	DIR *dirp;
	struct dirent *tmp;
	dirp = opendir(dir);
	while ((tmp = readdir(dirp)) != NULL) {
		if (strlen(tmp->d_name) /*d_namlen*/ == index.size() && strcmp(tmp->d_name, index.c_str()) == 0) {
			(void)closedir(dirp);
			return (1);
		}
	}
	(void)closedir(dirp);
	return 0;
}

void	config::serverData_check(t_serverData &sD){
	Socket sock;
	try{
		sock.listen(sD.listen);
	}
	catch (Socket::SocketException const &e){
		throw ValueError::ParsingException("incorrect listen value : " +  SSTR(sD.listen) + " must be available port");
	}
	// if (::listen(sD.listen, MAX_CONN) == -1 && std::cerr << sD.listen << std::endl)
	// 	throw ValueError::ParsingException("incorrect listen value");
	// sD.root 
	DIR *dirp;
	if ((dirp = opendir(sD.root.c_str())) == NULL /*chdir(sD.root.c_str()) == -1*/)
		throw ValueError::ParsingException("incorrect root value : " + sD.root + ", must be existing dir");
	else
		closedir(dirp);
	
	if (sD.autoindex != true && sD.autoindex != false)
		throw ValueError::ParsingException("incorrect autoindex : " + SSTR(sD.autoindex) + " must be on/off");
	if (sD.client_max_body_size <= 0)
		throw ValueError::ParsingException("incorrect client_max_body_size : " + SSTR(sD.client_max_body_size) + " must be > 0");
	int	check = 0;
	for ( std::list< std::string >::iterator it = sD.index.begin() ; it != sD.index.end() ; it++ ){
		check += config::sD_index_check(sD.root.c_str(), *it);
	}
	if (!check)
		throw ValueError::ParsingException("incorrect index value : no corresponding file found");
	

	for ( std::map< int, std::string >::iterator it = sD.error_page.begin() ; it != sD.error_page.end() ; it++ ){
		config::error_page_check(*it);
	}
	for ( std::list< t_locationData >::iterator it = sD.locations.begin() ; it != sD.locations.end() ; it++ ){
		config::locationData_check(*it);
	}
}

void	config::config_check( std::list< t_serverData > *_content ) throw( ParsingException ){
	// std::list< t_serverData >::iterator it_listen;
	std::list< std::string >::iterator it_name;
	std::map< std::string, bool > map_name;
	for (std::list< t_serverData >::iterator it = _content->begin() ; it != _content->end() ; it++){
		/*
			Not an error, several servers can listen on the same port
			http://nginx.org/en/docs/http/request_processing.html
		*/
		// it_listen = _content->begin();
		// while (it_listen != _content->end()){
		// 	// std::cout << SSTR(it_listen->listen) << std::endl;
		// 	if (it != it_listen && it_listen->listen == it->listen){
		// 		throw ValueError::ParsingException("two servers have the save listen value : " + SSTR(it->listen));
		// 	}
		// 	it_listen++;
		// }

// server_name check
		it_name = it->server_name.begin();
		while (it_name != it->server_name.end()){
			if (map_name.find(*it_name) != map_name.end()){
				throw ValueError::ParsingException("two servers have the save name value : " + *it_name);
			}
			else {
				map_name[*it_name] = true;
			}
			it_name++;
		}

		config::serverData_check(*it);
	}
}

std::list< t_serverData >	config::getContent() const
{
	return this->_content;
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
