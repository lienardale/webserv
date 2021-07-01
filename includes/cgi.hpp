/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:46 by akira             #+#    #+#             */
/*   Updated: 2021/07/01 17:03:05 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "parsing/dataStructure.hpp"
#include "webserv.hpp"
#include <iostream>
#include <string>

class cgi
{
  private:
    enum cgi_variables
    {
        // variables specifiques a la requete
        AUTH_TYPE,      // : AuthType Basic
        CONTENT_LENGTH, // size of body (empty if none) : Content-Length: <length>
        CONTENT_TYPE,
        PATH_INFO,
        PATH_TRANSLATED,
        QUERY_STRING, // everything after "?" in the url
        REMOTE_ADDR,
        REMOTE_HOST,
        REMOTE_IDENT,
        REMOTE_USER,
        REQUEST_METHOD, // GET
        SCRIPT_NAME,    // SCRIPT_FILENAME in config
        SERVER_PORT,    // Host: localhost:8000 -> in request
        SERVER_PROTOCOL,
        // variables en rapport avec le client
        GATEWAY_INTERFACE, //
        SERVER_NAME,       // "webserv" or "localhost" -> in config
        SERVER_SOFTWARE,   //
                           // variables provenant du client
        HTTP_ACCEPT,
        HTTP_ACCEPT_LANGUAGE,
        HTTP_USER_AGENT,
        HTTP_COOKIE,
        // scheme,
        // protocol_var_name,
        // protocol-var-name  = ( protocol | scheme ) "_" var-name
        // scheme             = alpha *( alpha | digit | "+" | "-" | "." )
        // var-name           = token
        // extension_var_name,
        // extension-var-name = token
        LEN_CGI_ENV
    };

    struct meta_var
    {
        std::string _auth_type;
        std::string _content_length;
        std::string _content_type;
        std::string _path_info;
        std::string _path_translated;
        std::string _query_string;
        std::string _remote_addr;
        std::string _remote_host;
        std::string _remote_ident;
        std::string _remote_user;
        std::string _request_method;
        std::string _script_name;
        std::string _server_port;
        std::string _server_protocol;
        std::string _gateway_interface;
        std::string _server_name;
        std::string _server_software;
        std::string _http_accept;
        std::string _http_accept_language;
        std::string _http_user_agent;
        std::string _http_cookie;
    };

    meta_var s_env;
    char *env[LEN_CGI_ENV + 1];
    cgi();

  public:
    cgi(Socket &sock, t_serverData &data) /* throw( cgi::CGIException )*/;
    cgi(const cgi &);
    ~cgi();
    cgi &operator=(const cgi &);
    void setCgi(Socket &sock, t_serverData &data);
    void setCgiMetaVar(Socket &sock, t_serverData &data);
    void setCgiEnv(void);
    std::string parseURI(std::string uri);
    char **getCgiEnv(void);

    // class CGIException : public std::exception
    // {
    // 	public:
    // 		const char *what( void ) const throw();
    // };
};
