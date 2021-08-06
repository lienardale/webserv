/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcariou <pcariou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 15:24:37 by pcariou           #+#    #+#             */
/*   Updated: 2021/08/06 15:28:33 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include "handleRequest.hpp"
#include "response.hpp"
#include "webserv.hpp"

http::Response handleCGI(const http::Request &request, const t_serverData &data, const t_locInfos &loc)
{
	http::Response ret = http::Response(http::OK);

	ret.setBodyCGI(cgi(request, loc, data).getOutput());
	return ret;
}
