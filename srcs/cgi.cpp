/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:47 by akira             #+#    #+#             */
/*   Updated: 2021/06/24 16:18:34 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

/******************************************************************************
 *				Constructeurs et Destructeurs
 *****************************************************************************/

cgi::cgi()
{

}

cgi::cgi( Socket &sock ) throw( cgi::CGIException )
{
	if (setCgiEnv(sock))
		throw( cgi::CGIException() );

}

cgi::~cgi()
{

}

cgi::cgi(const cgi& obj)
{
	*this = obj;
}

/******************************************************************************
 *			Operator overloading
 *****************************************************************************/

cgi&	cgi::operator=(const cgi& obj)
{
	(void)obj;
	return *this;
}

/******************************************************************************
 *					CGIException
 *****************************************************************************/
/*
 *	Le message correspond au message de errno
 */
const char *cgi::CGIException::what() const throw()
{
	return strerror( errno );
}

/******************************************************************************
 *							Fonction membres
 *****************************************************************************/

int cgi::setCgiEnv( Socket &sock )
{
	// std::cout<< sock.get_request() <<std::endl;
	std::fstream fs;
	fs.open("test.txt", std::fstream::out);
	fs << sock.get_request();
	// data->env[SERVER_SOFTWARE] = 
	// data->env[SERVER_NAME] = 
	// data->env[GATEWAY_INTERFACE] = 
	// data->env[SERVER_PROTOCOL] = 
	// data->env[SERVER_PORT] = 
	// data->env[PATH_INFO] = 
	// data->env[PATH_TRANSLATED] = 
	// data->env[SCRIPT_NAME] = 
	// data->env[QUERY_STRING] = 
	// data->env[REMOTE_HOST] = 
	// data->env[REMOTE_ADDR] = 
	// data->env[AUTH_TYPE] = 
	// data->env[REMOTE_USER] = 
	// data->env[REMOTE_IDENT] = 
	// data->env[CONTENT_TYPE] = 
	// data->env[CONTENT_LENGTH] = 
	// data->env[HTTP_ACCEPT] = 
	// data->env[HTTP_ACCEPT_LANGUAGE] = 
	// data->env[HTTP_USER_AGENT] = 
	// data->env[HTTP_COOKIE] = 
	env[LEN_CGI_ENV] = NULL;
	return 0;
}