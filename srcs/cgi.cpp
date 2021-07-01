/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:47 by akira             #+#    #+#             */
/*   Updated: 2021/07/01 16:53:18 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <stdlib.h>

/******************************************************************************
 *				Constructeurs et Destructeurs
 *****************************************************************************/

cgi::cgi()
{
}

cgi::cgi(Socket &sock, t_serverData &data) // throw( cgi::CGIException )
{
    setCgi(sock, data);
    // if (setCgi(sock))
    // 	throw( cgi::CGIException() );
}

cgi::~cgi()
{
}

cgi::cgi(const cgi &obj)
{
    *this = obj;
}

/******************************************************************************
 *			Operator overloading
 *****************************************************************************/

cgi &cgi::operator=(const cgi &obj)
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
 *					Geters
 *****************************************************************************/

char **cgi::getCgiEnv(void)
{
    return this->env;
}

/******************************************************************************
 *							Fonction membres
 *****************************************************************************/

std::string cgi::parseURI(std::string uri)
{
    size_t pos;

    if ((pos = uri.find("?")) != std::string::npos)
        return uri.substr(pos + 1, uri.size());
    return "";
}

void cgi::setCgiMetaVar(Socket &sock, t_serverData &data)
{
    //  char buffer [33];
    s_env._auth_type = "AUTH_TYPE=" + sock.get_m_request().header("AuthType");                 // ok
    s_env._content_length = "CONTENT_LENGTH=" + sock.get_m_request().header("Content-Length"); // ok
    s_env._content_type = "CONTENT_TYPE=" + sock.get_m_request().header("Content-Type");       // ok
    s_env._path_info = "PATH_INFO=" + sock.get_infos()[1];                                     // ok
    s_env._path_translated = "PATH_TRANSLATED=" + SSTR(getenv("PWD")) + sock.get_infos()[1];   // ok
    s_env._query_string = "QUERY_STRING=" + parseURI(sock.get_m_request().uri());              // ok
    s_env._remote_addr = "REMOTE_ADDR=127.0.0.1";                                              // ok
    s_env._remote_host = "REMOTE_HOST=" + sock.get_m_request().header("Host");                 // ok
    s_env._remote_ident = "REMOTE_IDENT=user_id";                                              // ok
    s_env._remote_user = "REMOTE_USER=user_name";                                              // ok
    s_env._request_method = "REQUEST_METHOD=" + sock.get_m_request().method();                 // ok
    s_env._script_name = "SCRIPT_NAME=/cgi-bin/php-cgi"; // FAST_CGI_CONF
    s_env._server_port = "SERVER_PORT=8000" /* + itoa(data.listen,buffer,10)*/;
    s_env._server_protocol = "SERVER_PROTOCOL=" + sock.get_m_request().protocol();                          // ok
    s_env._redirect_status = "REDIRECT_STATUS=200";
    s_env._gateway_interface = "GATEWAY_INTERFACE=CGI/1.1";                                                 // ok
    s_env._server_name = "SERVER_NAME=" + data.addr_ip;                                                     // ok
    s_env._server_software = "SERVER_SOFTWARE=Nginx/2.0";                                                   // ok
    s_env._http_accept = "HTTP_ACCEPT=" + sock.get_m_request().header("Accept");                            // ok
    s_env._http_accept_language = "HTTP_ACCEPT_LANGUAGE=" + sock.get_m_request().header("Accept-Language"); // ok
    s_env._http_user_agent = "HTTP_USER_AGENT=" + sock.get_m_request().header("User-Agent");                // ok
    s_env._http_cookie = "HTTP_COOKIE=" + sock.get_m_request().header("                                            ");
}

void cgi::setCgiEnv(void)
{
    env[AUTH_TYPE] = const_cast<char *>(s_env._auth_type.c_str());
    env[CONTENT_LENGTH] = const_cast<char *>(s_env._content_length.c_str());
    env[CONTENT_TYPE] = const_cast<char *>(s_env._content_type.c_str());
    env[PATH_INFO] = const_cast<char *>(s_env._path_info.c_str());
    env[PATH_TRANSLATED] = const_cast<char *>(s_env._path_translated.c_str());
    env[QUERY_STRING] = const_cast<char *>(s_env._query_string.c_str());
    env[REMOTE_ADDR] = const_cast<char *>(s_env._remote_addr.c_str());
    env[REMOTE_HOST] = const_cast<char *>(s_env._remote_host.c_str());
    env[REMOTE_IDENT] = const_cast<char *>(s_env._remote_ident.c_str());
    env[REMOTE_USER] = const_cast<char *>(s_env._remote_user.c_str());
    env[REQUEST_METHOD] = const_cast<char *>(s_env._request_method.c_str());
    env[SCRIPT_NAME] = const_cast<char *>(s_env._script_name.c_str());
    env[SERVER_PORT] = const_cast<char *>(s_env._server_port.c_str());
    env[SERVER_PROTOCOL] = const_cast<char *>(s_env._server_protocol.c_str());
    env[REDIRECT_STATUS] = const_cast<char *>(s_env._redirect_status.c_str()); 
    env[GATEWAY_INTERFACE] = const_cast<char *>(s_env._gateway_interface.c_str());
    env[SERVER_NAME] = const_cast<char *>(s_env._server_name.c_str());
    env[SERVER_SOFTWARE] = const_cast<char *>(s_env._server_software.c_str());
    env[HTTP_ACCEPT] = const_cast<char *>(s_env._http_accept.c_str());
    env[HTTP_ACCEPT_LANGUAGE] = const_cast<char *>(s_env._http_accept_language.c_str());
    env[HTTP_USER_AGENT] = const_cast<char *>(s_env._http_user_agent.c_str());
    env[HTTP_COOKIE] = const_cast<char *>(s_env._http_cookie.c_str());
    env[LEN_CGI_ENV] = NULL;
}

void cgi::setCgi(Socket &sock, t_serverData &data)
{
    // (void) sock;
    // (void) data;
    setCgiMetaVar(sock, data);
    setCgiEnv();
    // for (int i = 0; env[i]; i++){
    // 	std::cout << "env["<<i<<"] = |" << env[i]<<"|" << std::endl;
    // }
}