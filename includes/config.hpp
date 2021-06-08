#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "webserv.hpp"

class config {
	public:
		config(void);
		config(std::string server, int port);
		config(const config &src);
		~config(void);
		config	&operator=(const config &rhs);
		std::string	getServer(void) const;
		int	getPort(void) const;
		int ft_print_config(void);

	private:
		std::string	server;
		int	port;
};

#endif
