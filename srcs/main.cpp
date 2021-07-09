/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/09 20:19:59 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

http::Server SERVER;

void handleSignal(int sig)
{
    (void)sig;

    SERVER.stop();
}

int main(int ac, char **av, char **env)
{
    (void)env;
    if (ac > 2)
    {
        std::cerr << "Too many arguments, max 1 (config file)." << std::endl;
    }
    try
    {
        if (ac == 1)
            av[1] = NULL;
        config conf(av[1]);

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
