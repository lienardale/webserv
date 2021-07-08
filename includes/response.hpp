/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 17:13:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/07 19:39:48 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "socket.hpp"
#include "statusCode.hpp"
#include <iostream>
#include <map>

namespace http
{
class Response
{
  public:
    Response(http::Status code);
    Response(const Response &other);
    Response &operator=(const Response &other);
    ~Response(void);

    /**************************************************************************
     *			Setters
     *************************************************************************/
    void setHeader(const std::string key, const std::string value);
    void setBody(const std::string &content, const std::string mimetype);
    void setCode(const http::Status code);

    /**************************************************************************
     *			Fonctions membres
     *************************************************************************/
    std::string toString(void) const;

  private:
    http::Status _code;
    std::string _status;
    std::map< std::string, std::string > _headers;
    std::pair< std::string, std::string > _body;
};
} // namespace http

#endif