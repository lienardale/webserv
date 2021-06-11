/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverKeys.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:59:41 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/11 17:01:15 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERKEYS_HPP
#define SERVERKEYS_HPP

#include "dataStructure.hpp"
#include <istream>

void handleListen( t_serverData &server, const std::string value ) throw( std::exception );

void handleServerName( t_serverData &server, const std::string value ) throw( std::exception );

void handleAutoindex( t_serverData &server, const std::string value ) throw( std::exception );

void handleRoot( t_serverData &server, const std::string value ) throw( std::exception );

void handleClientMaxBody( t_serverData &server, const std::string value ) throw( std::exception );
void handleIndex( t_serverData &server, const std::string value ) throw( std::exception );

void handleErrorPage( t_serverData &server, const std::string value ) throw( std::exception );
#endif
