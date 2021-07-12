/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:47:47 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/10 12:14:56 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLEMETHOD_HPP
#define HANDLEMETHOD_HPP

#include "parsing/dataStructure.hpp"
#include "request.hpp"
#include "response.hpp"

http::Response handleRequest(const http::Request &request, const t_serverData &data);

http::Response handleGET(const http::Request &request, const t_locationData &data);

http::Response handlePOST(const http::Request &request, const t_locationData &data);

http::Response handleDELETE(const http::Request &request, const t_locationData &data);

#endif
