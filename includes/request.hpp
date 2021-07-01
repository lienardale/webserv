/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 17:05:36 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/01 13:05:03 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "parsing/parsingExceptions.hpp"
#include <iostream>
#include <map>

class Request {
public:
  Request(void); // Private default constructor
  Request(std::string content) throw(ParsingException);
  Request(const Request &other);
  Request &operator=(const Request &other);
  ~Request(void);

  /**************************************************************************
   *				Fonctions membres
   *************************************************************************/
  std::string method(void) const;
  std::string uri(void) const;
  std::string protocol(void) const;
  std::string host(void) const;
  std::string header(const std::string key) const;

private:
  std::string _method;
  std::string _uri;
  std::string _protocol;
  std::string _host;
  std::map<std::string, std::string> _headers;
};

#endif