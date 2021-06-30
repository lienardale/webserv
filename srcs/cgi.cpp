/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:47 by akira             #+#    #+#             */
/*   Updated: 2021/06/30 10:53:02 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

/******************************************************************************
 *				Constructeurs et Destructeurs
 *****************************************************************************/

cgi::cgi()
{

}

cgi::cgi( Socket &sock ) // throw( cgi::CGIException )
{
	setCgi(sock);
	// if (setCgi(sock))
	// 	throw( cgi::CGIException() );
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
// const char *cgi::CGIException::what() const throw()
// {
// 	return strerror( errno );
// }

/******************************************************************************
 *							Fonction membres
 *****************************************************************************/

	// std::fstream fs;
	// fs.open("test.txt", std::fstream::out);
	// fs << sock.get_request();
	// fs.close();
	// std::string request;
	// std::string found;
	// size_t pos;
	// // size_t npos;
	// size_t i;
	// pos = 0;
	// i = 0;
	// request = sock.get_request();

	// while (i < LEN_CGI_ENV - 1 && (pos = request.find(" ")) != std::string::npos)
	// {
	// 	std::cout << "\nPOS " << i << " : " << pos << std::endl;
	// 	// if (i == LEN_CGI_ENV)
	// 	// 	break ;
	// 	env[i] = const_cast<char*>(request.substr(0, pos).c_str());
	// 	request.erase(0, pos + 1);
	// 	std::cout << i << " | " << env[i] << std::endl;
	// 	i++;
	// }
	// env[LEN_CGI_ENV] = NULL;


void cgi::setCgiMetaVar( Socket &sock )
{
	s_env._auth_type = "AUTH_TYPE=" + sock.get_m_request().header( "AuthType" );
	s_env._content_length = "CONTENT_LENGTH=" + sock.get_m_request().header( "Content-Length" );
	s_env._content_type = "CONTENT_TYPE=" + sock.get_m_request().header( "Content-Type" );
	s_env._path_info = "PATH_INFO=" + sock.get_locationData().path;
	s_env._path_translated = "PATH_TRANSLATED=" + sock.get_m_request().header( "      " );
	s_env._query_string = "QUERY_STRING=" + sock.get_m_request().uri();
	s_env._remote_addr = "REMOTE_ADDR=" + sock.get_m_request().header( "              " );
	s_env._remote_host = "REMOTE_HOST=" + sock.get_m_request().host();
	s_env._remote_ident = "REMOTE_IDENT=" + sock.get_m_request().header( "            " );
	s_env._remote_user = "REMOTE_USER=" + sock.get_m_request().header( "              " );
	s_env._request_method = "REQUEST_METHOD=" + sock.get_m_request().method();
	s_env._script_name = "SCRIPT_NAME=" + sock.get_m_request().header( "              " );
	s_env._server_port = "SERVER_PORT=" + sock.get_m_request().header( "              " );
	s_env._server_protocol = "SERVER_PROTOCOL=" + sock.get_m_request().protocol();
	s_env._gateway_interface = "GATEWAY_INTERFACE=" + sock.get_m_request().header( "AuthType" );
	s_env._server_name = "SERVER_NAME=" + sock.get_m_request().header( "              " );
	s_env._server_software = "SERVER_SOFTWARE=" + sock.get_m_request().header( "      " );
	s_env._http_accept = "HTTP_ACCEPT=" + sock.get_m_request().header( "Accept" );
	s_env._http_accept_language = "HTTP_ACCEPT_LANGUAGE=" + sock.get_m_request().header( "Accept-Language" );
	s_env._http_user_agent = "HTTP_USER_AGENT=" + sock.get_m_request().header( "User-Agent" );
	s_env._http_cookie = "HTTP_COOKIE=" + sock.get_m_request().header( "AuthType" );
}

void cgi::setCgiEnv( void )
{
	env[AUTH_TYPE] = s_env._auth_type.c_str();
	env[CONTENT_LENGTH] = s_env._content_length.c_str();
	env[CONTENT_TYPE] = s_env._content_type.c_str();
	env[PATH_INFO] = s_env._path_info.c_str();
	env[PATH_TRANSLATED] = s_env._path_translated.c_str();
	env[QUERY_STRING] = s_env._query_string.c_str();
	env[REMOTE_ADDR] = s_env._remote_addr.c_str();
	env[REMOTE_HOST] = s_env._remote_host.c_str();
	env[REMOTE_IDENT] = s_env._remote_ident.c_str();
	env[REMOTE_USER] = s_env._remote_user.c_str();
	env[REQUEST_METHOD] = s_env._request_method.c_str();
	env[SCRIPT_NAME] = s_env._script_name.c_str();
	env[SERVER_PORT] = s_env._server_port.c_str();
	env[SERVER_PROTOCOL] = s_env._server_protocol.c_str();
	env[GATEWAY_INTERFACE] = s_env._gateway_interface.c_str();
	env[SERVER_NAME] = s_env._server_name.c_str();
	env[SERVER_SOFTWARE] = s_env._server_software.c_str();
	env[HTTP_ACCEPT] = s_env._http_accept.c_str();
	env[HTTP_ACCEPT_LANGUAGE] = s_env._http_accept_language.c_str();
	env[HTTP_USER_AGENT] = s_env._http_user_agent.c_str();
	env[HTTP_COOKIE] = s_env._http_cookie.c_str();
	env[LEN_CGI_ENV] = NULL;
}

void cgi::setCgi( Socket &sock )
{
	setCgiMetaVar( sock );
	setCgiEnv();
}