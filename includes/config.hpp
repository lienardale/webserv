#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <iostream>

class config {
	public:
		config(void);
		config(std::string server, int port);
		config(const config &src);
		~config(void);
		config	&operator=(const config &rhs);
		std::string	getServer(void) const;
		int	getPort(void) const;

	private:
		std::string	server;
		int	port;
};

#endif
