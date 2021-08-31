/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtrim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcariou <pcariou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 16:55:27 by pcariou           #+#    #+#             */
/*   Updated: 2021/08/31 16:57:22 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// trim from end of string (right)
std::string rtrim(std::string s, const char *t)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
std::string ltrim(std::string s, const char *t)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

std::string strtrim(std::string s, const char *t)
{
    return ltrim(rtrim(s, t), t);
}
