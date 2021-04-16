/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcariou <pcariou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 13:29:08 by pcariou           #+#    #+#             */
/*   Updated: 2021/04/16 14:38:03 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <string>
# include <iostream>
# include <vector>

class Socket {
	public:
		Socket(void) {};
		Socket(int fd) : fd(fd) {};
		Socket(const Socket &src);
		~Socket(void) {};
		Socket	&operator=(const Socket &rhs);
		int	getFd(void) const;

//	private:
		int			fd;//fd creation
		sockaddr_in	sockaddr;
		int			connect;//fd connection
		std::vector<std::string>	infos;//http request infos
};

#endif
