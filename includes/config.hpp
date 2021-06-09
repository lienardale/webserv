#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "webserv.hpp"

class config {
	public:
		class ConfigFileException : public std::exception {
			public:
				ConfigFileException(void) throw()
				{
					return ;
				}
				ConfigFileException(const ConfigFileException &src) throw();
				virtual ~ConfigFileException(void) throw()
				{
					return ;
				}
				ConfigFileException   &operator=(const ConfigFileException &rhs) throw();
				virtual const char *what() const throw()
				{
					return ("Error: Invalid config file (webserv.conf)");
				}
		};
		config(void);
		config(std::string server, int port);
		config(const config &src);
		~config(void);
		config	&operator=(const config &rhs);
		std::string	getServer(void) const;
		int	getPort(void) const;
		//int ft_print_config(void);

	private:
		std::string	server;
		int	port;
};

#endif