/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/18 12:00:04 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

http::Server SERVER;

void handleSignal( int sig )
{
	( void )sig;

	SERVER.stop();
}

int main( void )
{
	try
	{
		config conf;

		// Signal handling
		std::signal( SIGINT, handleSignal );
		std::signal( SIGTERM, handleSignal );
		std::signal( SIGQUIT, handleSignal );

		// list dataserv
		SERVER.init( conf.getContent(), 1 );
	}
	catch ( config::ConfigFileException const &e )
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
