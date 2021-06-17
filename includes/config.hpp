#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "parsing/parser.hpp"
#include <iostream>

class config
{
  public:
	class ConfigFileException : public std::exception
	{
	  public:
		ConfigFileException( void ) throw()
		{
			return;
		}
		ConfigFileException( const ConfigFileException &src ) throw();
		virtual ~ConfigFileException( void ) throw()
		{
			return;
		}
		ConfigFileException &operator=( const ConfigFileException &rhs ) throw();
		virtual const char *what() const throw()
		{
			return ( "Error: Invalid config file (webserv.conf)" );
		}
	};
	config( void );
	config( char *path );
	config( std::string server, int port );
	config( const config &src );
	~config( void );
	config &operator=( const config &rhs );
	std::string getServer( void ) const;
	int getPort( void ) const;
	void	config_check( std::list< t_serverData > *_content ) throw( ParsingException );
	void	serverData_check(t_serverData &sD);
	void	sD_index_check(std::string &index);
	void	server_name_check(std::string &server_name);
	void	error_page_check(std::pair< const int, std::string > &error_page);
	void	locationData_check(t_locationData &lD);
	void	lD_index_check(std::string &index);
	void	fastcgi_param_check( std::pair< const std::string, std::string > &fcgi);
	void	methods_check(std::string &method);
	// int ft_print_config(void);

  private:
	std::string server;
	int port;
	std::list< t_serverData > _content;
};

#endif
