/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 13:10:16 by alienard          #+#    #+#             */
/*   Updated: 2021/07/01 13:10:16 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static int ft_len(int value, int base)
{
    int len;
    unsigned int v;

    len = value > 0 ? 0 : 1;
    v = value > 0 ? value : -value;
    while (v > 0)
    {
        v /= base;
        len++;
    }
    return len;
}

char *itoa(int value, char *str, int base)
{
    int len = ft_len(value, base);
    unsigned int v;
    int i = 0;

    v = value > 0 ? value : -value;
    if (value < 0)
        str[i++] = '-';
    str[len] = '\0';
    while (i <= --len)
    {
        str[len] = v % base + '0';
        v /= base;
    }
    return str;
}
