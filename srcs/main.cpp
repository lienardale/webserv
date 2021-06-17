/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/17 15:44:40 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

http::Server SERVER(8000, 10);

void handleSignal(int sig)
{
	(void)sig;
	SERVER.stop();
}

int main(int ac, char **av)
{

	if (ac > 2){
		std::cerr << "Too many arguments, max 1 (config file)." << std::endl;
	}
	// Signal handling
	std::signal(SIGINT, handleSignal);
	std::signal(SIGTERM, handleSignal);
	std::signal(SIGQUIT, handleSignal);
	
	try {
		if (ac == 2)
			config	conf(av[1]);
		else if (ac == 1)
			config	conf;
		//conf.ft_print_config();
		SERVER.listen();
	}
	catch (config::ConfigFileException const &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
