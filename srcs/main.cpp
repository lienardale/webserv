/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/17 18:29:39 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::list< http::Server >	SERVERS;

void handleSignal(int sig)
{
	(void)sig;

	for (std::list< http::Server >::iterator it=SERVERS.begin(); it != SERVERS.end(); ++it)
		it->stop();
}

int main(int ac, char **av, char **env)
{
	// chdir("..");
	(void)env;
	if (ac > 2){
		std::cerr << "Too many arguments, max 1 (config file)." << std::endl;
	}
	try {
		if (ac == 1)
			av[1] = NULL;
		config	conf(av[1]);

		// Signal handling
		std::signal(SIGINT, handleSignal);
		std::signal(SIGTERM, handleSignal);
		std::signal(SIGQUIT, handleSignal);
		
		// list dataserv
		std::list< t_serverData >	data = conf.getContent();

		//create servers and push them in list (SERVERS)
		for (std::list< t_serverData >::iterator it=data.begin(); it != data.end(); ++it)	
			SERVERS.push_back(http::Server(it->listen, it->client_max_body_size));
		//iter through SERVERS and listen each of them
		for (std::list< http::Server >::iterator it=SERVERS.begin(); it != SERVERS.end(); ++it)
			it->listen();
	}
	catch (config::ConfigFileException const &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
