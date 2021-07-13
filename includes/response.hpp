/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 17:13:23 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/13 14:58:20 by dboyer           ###   ########.fr       */
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
    Response(void);
    Response(http::Status code);
    Response(const Response &other);
    Response &operator=(const Response &other);
    ~Response(void);

    /**************************************************************************
     *			Setters
     *************************************************************************/
    void setHeader(const std::string key, const std::string value);
    void setBody(const std::string &content, const std::string mimetype);
    void setBodyCGI(const std::string &content);
    void setCode(const http::Status code);
    /**************************************************************************
     *			Getters
     *************************************************************************/
    int code(void) const;

    /**************************************************************************
     *			Fonctions membres
     *************************************************************************/
    std::string toString(void);
    std::string toString(const std::map< int, std::string > &errorPages);

  private:
    http::Status _code;
    std::string _status;
    std::map< std::string, std::string > _headers;
    std::pair< std::string, std::string > _body;
    std::string _bodyCGI;
};
} // namespace http

#endif
