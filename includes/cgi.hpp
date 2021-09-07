/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard@student.42.fr <alienard>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:46 by akira             #+#    #+#             */
/*   Updated: 2021/09/06 17:54:43 by alienard@st      ###   ########.fr       */
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
        AUTH_TYPE,
        CONTENT_LENGTH,
        CONTENT_TYPE,
        PATH_INFO,
        PATH_TRANSLATED,
        QUERY_STRING,
        REMOTE_ADDR,
        REMOTE_HOST,
        REMOTE_IDENT,
        REMOTE_USER,
        REQUEST_METHOD,
        REQUEST_URI,
        SCRIPT_NAME,
        SCRIPT_FILENAME, 
        SERVER_PORT,
        SERVER_PROTOCOL,
        REDIRECT_STATUS,
        GATEWAY_INTERFACE,
        SERVER_NAME,
        SERVER_SOFTWARE,
        HTTP_ACCEPT,
        HTTP_ACCEPT_LANGUAGE,
        HTTP_USER_AGENT,
        HTTP_COOKIE,
		UPLOAD_DIR,
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
        std::string _request_uri;
        std::string _script_name;
        std::string _script_file_name;
        std::string _server_port;
        std::string _server_protocol;
        std::string _redirect_status;
        std::string _gateway_interface;
        std::string _server_name;
        std::string _server_software;
        std::string _http_accept;
        std::string _http_accept_language;
        std::string _http_user_agent;
        std::string _http_cookie;
		std::string	_upload_dir;
        // std::string _;
    };

    meta_var s_env;
    char *env[LEN_CGI_ENV + 1];
	std::string	_output;
    cgi();

  public:
    cgi(const http::Request &request, const t_locInfos &loc, const t_serverData &dataserv, std::string file)/* throw( cgi::CGIException )*/;;
    cgi(const cgi &);
    ~cgi();
    cgi &operator=(const cgi &);
	void setCgi(const http::Request &request, const t_locInfos &loc, const t_serverData &dataserv, std::string file);
	void setCgiMetaVar(const http::Request &request, const t_locInfos &loc, const t_serverData &data, std::string file);
    void setCgiEnv(void);
    std::string parseURI(std::string uri);
    char **getCgiEnv(void);
	void Cgi(const http::Request &request, const t_locInfos &loc, const t_serverData &data_serv, std::string file);
	std::string	getOutput() const;

    class CGIException : public std::exception
    {
    	public:
    		const char *what( void ) const throw();
    };
};
