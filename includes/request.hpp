/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 17:05:36 by dboyer            #+#    #+#             */
/*   Updated: 2021/09/08 12:06:58 by dboyer           ###   ########.fr       */
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
    std::string header(const std::string key) const;
    std::map< std::string, std::string > getHeader(void) const;
    bool keepAlive(void) const;
    bool isFinished(void) const;
    bool isBodyTooLarge(void) const;

    /**************************************************************************
     *				Getters
     *************************************************************************/

    void setHeaderPath(std::string path);

    /**************************************************************************
     *				Fonctions membres
     *************************************************************************/

    void parse(const std::string &content, int maxBodySize);
    void clear(void);

  private:
    bool _isBody;
    bool _finish;
    bool _isBodyTooLarge;
    std::string _buffer;
    std::map< std::string, std::string > _headers;

    void _extract(std::string &content);
    void _extractHeader(std::string &content);
    void _extractBody(int maxBodySize);
};

} // namespace http

std::ostream &operator<<(std::ostream &os, const http::Request &r);

#endif
