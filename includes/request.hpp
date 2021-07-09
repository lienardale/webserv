/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 17:05:36 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/09 20:19:29 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "parsing/parsingExceptions.hpp"
#include <iostream>
#include <map>

namespace http
{
class Request
{
  public:
    Request(void);
    Request(const Request &other);
    Request &operator=(const Request &other);
    ~Request(void);

    /**************************************************************************
     *				Getters
     *************************************************************************/
    std::string method(void) const;
    std::string uri(void) const;
    std::string protocol(void) const;
    std::map< std::string, std::string > getHeader(void) const;
    bool keepAlive(void) const;
    bool isFinished(void) const;

    /**************************************************************************
     *				Fonctions membres
     *************************************************************************/
    std::string header(const std::string key) const;
    void parse(const std::string &content) throw(ParsingException);
    void clear(void);

  private:
    bool _isBody;
    bool _finish;
    std::string _buffer;
    std::string _method;
    std::string _uri;
    std::string _protocol;
    std::string _host;
    std::map< std::string, std::string > _headers;

    void _extract(std::string &content) throw(ParsingException);
    void _extractHeader(std::string &content) throw(ParsingException);
    void _extractBody(void) throw(ParsingException);
};

} // namespace http

std::ostream &operator<<(std::ostream &os, const http::Request &r);

#endif
