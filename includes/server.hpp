/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 10:27:31 by dess              #+#    #+#             */
/*   Updated: 2021/04/16 10:32:49 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <inttypes.h>

class Server
{

  public:
	Server(void);
	Server(uint32_t port, uint32_t timeout);
};

#endif
