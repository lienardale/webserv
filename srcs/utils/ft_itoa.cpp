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

#include "utils.hpp"

static int ft_len(int value, int base)
{
	int len;
	unsigned int v;

	len = 0;
	if (v < 0)
		len++;

	return len;
}

char *  itoa ( int value, char * str, int base )
{
	return str;
}