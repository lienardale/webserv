/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/08 16:25:11 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

http::Server SERVER(8000, 10);

void handleSignal(int sig)
{
	(void)sig;
	SERVER.stop();
}

int main(void)
{
	// Signal handling
	std::signal(SIGINT, handleSignal);
	std::signal(SIGTERM, handleSignal);
	std::signal(SIGQUIT, handleSignal);
	
	try {
		config	conf;
		//conf.ft_print_config();
		SERVER.listen();
	}
	catch (config::ConfigFileException const &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
