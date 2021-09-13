/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingExceptions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 15:44:07 by dboyer            #+#    #+#             */
/*   Updated: 2021/09/08 12:06:30 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSINGEXCEPTIONS_HPP
#define PARSINGEXCEPTIONS_HPP
#include <exception>
#include <iostream>

class ParsingException : public std::exception
{
  public:
    ParsingException(const std::string msg);
    virtual ~ParsingException() throw();

    const char *what(void) const throw();

  private:
    std::string _msg;
};

class SyntaxError : public ParsingException
{
  public:
    SyntaxError(std::string msg);
    virtual ~SyntaxError() throw();

    const char *what(void) const throw();

  private:
    std::string _msg;
};

class ValueError : public ParsingException
{
  public:
    ValueError(std::string msg);
    virtual ~ValueError() throw();

    const char *what(void) const throw();

  private:
    std::string _msg;
};

class BadRequest : public ParsingException
{
  public:
    BadRequest(std::string msg);
    virtual ~BadRequest() throw();

    const char *what(void) const throw();

  private:
    std::string _msg;
};

class Forbidden : public ParsingException
{
  public:
    Forbidden(std::string msg);
    virtual ~Forbidden() throw();

    const char *what(void) const throw();

  private:
    std::string _msg;
};
#endif
