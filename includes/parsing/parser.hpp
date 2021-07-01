/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 16:26:10 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/01 16:56:34 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

#include "dataStructure.hpp"
#include "parsingExceptions.hpp"
#include "utils.hpp"
#include <exception>
#include <fstream>
#include <iostream>

#define RED "\033[31m"   /* Red */
#define GREEN "\033[32m" /* Green */
#define WHITE "\033[37m" /* White */

class Parser
{
  public:
    Parser(std::fstream &fs);
    Parser(const Parser &other);
    Parser &operator=(const Parser &other);
    ~Parser(void);

    std::list< t_serverData > parse(void) throw(ParsingException);

  private:
    std::string _content;
};

#endif
