/* **************************************************************************
 */
/*                                                                            */
/*                                                        :::      :::::::: */
/*   main.cpp                                           :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+ */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+ */
/*                                                +#+#+#+#+#+   +#+ */
/*   Created: 2021/03/09 12:15:04 by dboyer            #+#    #+# */
/*   Updated: 2021/09/07 21:44:08 by dboyer           ###   ########.fr */
/*                                                                            */
/* **************************************************************************
 */

#include "webserv.hpp"

http::Server SERVER;

void handleSignal(int sig)
{
    (void)sig;

    SERVER.stop();
}

void handleSIGTERM(int sig)
{
    (void)sig;
    SERVER.pause();
}

int main(int ac, char **av, char **env)
{
    (void)env;
    std::map< std::string, std::string > mimeTypes;

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
        std::signal(SIGTERM, handleSIGTERM);
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
