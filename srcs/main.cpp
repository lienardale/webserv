/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/08/02 19:15:22 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

http::Server SERVER;

void handleSignal(int sig)
{
	(void)sig;

	SERVER.stop();
}

void setMimeTypes(std::map<std::string, std::string> *mimeTypes)
{
	std::ifstream file("mime.types");
	std::string	line;
	std::string value;

	(void)mimeTypes;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::istringstream iss(line);
			std::vector<std::string> result;

			for (std::string s; iss >> s;)
				result.push_back(s);
			value = result.front();
			for (std::vector<std::string>::iterator it = result.begin(); it != result.end() ; ++it)
			{
				if (*it != *result.begin())
					(*mimeTypes)[*it] = value;
			}
			value.clear();
		}
		file.close();
	}	
}

int main(int ac, char **av, char **env)
{
	(void)env;
	std::map<std::string, std::string> mimeTypes;

	if (ac > 2)
	{
		std::cerr << "Too many arguments, max 1 (config file)." << std::endl;
	}
	try
	{
		if (ac == 1)
			av[1] = NULL;
		config conf(av[1]);
		setMimeTypes(&mimeTypes);

		//print out map
		for (std::map<std::string, std::string >::const_iterator it = mimeTypes.begin();
	    	it != mimeTypes.end(); ++it)
		    	std::cout << it->first << ": " << it->second  << std::endl;
		//

		// Signal handling
		std::signal(SIGINT, handleSignal);
		std::signal(SIGTERM, handleSignal);
		std::signal(SIGQUIT, handleSignal);

		// list dataserv
		SERVER.init(conf.getContent(), 1);
		SERVER.listen();
	}
	catch (config::ConfigFileException const &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
