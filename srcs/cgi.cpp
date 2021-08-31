/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard@student.42.fr <alienard>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/24 15:07:47 by akira             #+#    #+#             */
/*   Updated: 2021/08/31 14:16:26 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************
 *				Constructeurs et Destructeurs
 *****************************************************************************/

cgi::cgi()
{
}

cgi::cgi(const http::Request &request, const t_locInfos &loc,
         const t_serverData &dataserv, std::string file) // throw( cgi::CGIException )
{
    setCgi(request, loc, dataserv, file);
    Cgi(request, loc, dataserv, file);
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

std::string cgi::strtrim(std::string s, const char *t)
{
    return ltrim(rtrim(s, t), t);
}

void cgi::setCgiMetaVar(const http::Request &request, const t_locInfos &loc, const t_serverData &data, std::string file)
{
    char buffer[33];
    std::string cgi_script = SSTR("/") + loc._fastcgiParam;
    bzero(buffer, sizeof(buffer));
    s_env._auth_type = "AUTH_TYPE=" + request.header("AuthType");                        // ok
    s_env._content_length = "CONTENT_LENGTH=" + request.header("Content-Length");        // ok
    if (request.header("Method") == "POST")
        s_env._content_type = "CONTENT_TYPE=" + request.header("Content-Type");
    else
        s_env._content_type = "CONTENT_TYPE=" + mimeTypes(file, data);              // ok
    s_env._path_info = "PATH_INFO=" + file; // ok
    s_env._path_translated =
        "PATH_TRANSLATED=" + SSTR(getenv("PWD")) + "/" + strtrim(data.root, "/") + request.header("Path"); // ok
    s_env._query_string = "QUERY_STRING=" + request.header("query");                                       // ok
    s_env._remote_addr = "REMOTE_ADDR=127.0.0.1";                                                          // ok
    s_env._remote_host = "REMOTE_HOST=" + request.header("Host");                                          // ok
    s_env._remote_ident = "REMOTE_IDENT=user_id";                                                          // ok
    s_env._remote_user = "REMOTE_USER=user_name";                                                          // ok
    s_env._request_method = "REQUEST_METHOD=" + request.header("Method");                                  // ok
    s_env._request_uri = "REQUEST_URI=" + request.header("Path") + ((request.header("Query").empty()) ? "" : "?" ) + request.header("Query");                                           // ok
    s_env._script_name = "SCRIPT_NAME=" + file; // FAST_CGI_CONF
    s_env._script_file_name = "SCRIPT_FILENAME=" + file;
    s_env._server_port = "SERVER_PORT=" + SSTR(data.listen);             /*+ SSTR(itoa(data.listen,buffer,10));*/
    s_env._server_protocol = "SERVER_PROTOCOL=" + request.header("Protocol"); // ok
    s_env._redirect_status = "REDIRECT_STATUS=200";
    s_env._gateway_interface = "GATEWAY_INTERFACE=CGI/1.1";                                    // ok
    s_env._server_name = "SERVER_NAME=" + data.server_name.front();                                                // ok
    s_env._server_software = "SERVER_SOFTWARE=Nginx/2.0";                                      // ok
    s_env._http_accept = "HTTP_ACCEPT=" + request.header("Accept");                            // ok
    s_env._http_accept_language = "HTTP_ACCEPT_LANGUAGE=" + request.header("Accept-Language"); // ok
    s_env._http_user_agent = "HTTP_USER_AGENT=" + request.header("User-Agent");                // ok
    s_env._http_cookie = "HTTP_COOKIE=" + request.header("cookie");
}

void cgi::setCgiEnv(void)
{
    env[AUTH_TYPE] = const_cast< char * >(s_env._auth_type.c_str());
    env[CONTENT_LENGTH] = const_cast< char * >(s_env._content_length.c_str());
    env[CONTENT_TYPE] = const_cast< char * >(s_env._content_type.c_str());
    env[PATH_INFO] = const_cast< char * >(s_env._path_info.c_str());
    env[PATH_TRANSLATED] = const_cast< char * >(s_env._path_translated.c_str());
    env[QUERY_STRING] = const_cast< char * >(s_env._query_string.c_str());
    env[REMOTE_ADDR] = const_cast< char * >(s_env._remote_addr.c_str());
    env[REMOTE_HOST] = const_cast< char * >(s_env._remote_host.c_str());
    env[REMOTE_IDENT] = const_cast< char * >(s_env._remote_ident.c_str());
    env[REMOTE_USER] = const_cast< char * >(s_env._remote_user.c_str());
    env[REQUEST_METHOD] = const_cast< char * >(s_env._request_method.c_str());
    env[REQUEST_URI] = const_cast< char * >(s_env._request_uri.c_str());
    env[SCRIPT_NAME] = const_cast< char * >(s_env._script_name.c_str());
    env[SCRIPT_FILENAME] = const_cast< char * >(s_env._script_file_name.c_str());
    env[SERVER_PORT] = const_cast< char * >(s_env._server_port.c_str());
    env[SERVER_PROTOCOL] = const_cast< char * >(s_env._server_protocol.c_str());
    env[REDIRECT_STATUS] = const_cast< char * >(s_env._redirect_status.c_str());
    env[GATEWAY_INTERFACE] = const_cast< char * >(s_env._gateway_interface.c_str());
    env[SERVER_NAME] = const_cast< char * >(s_env._server_name.c_str());
    env[SERVER_SOFTWARE] = const_cast< char * >(s_env._server_software.c_str());
    env[HTTP_ACCEPT] = const_cast< char * >(s_env._http_accept.c_str());
    env[HTTP_ACCEPT_LANGUAGE] = const_cast< char * >(s_env._http_accept_language.c_str());
    env[HTTP_USER_AGENT] = const_cast< char * >(s_env._http_user_agent.c_str());
    env[HTTP_COOKIE] = const_cast< char * >(s_env._http_cookie.c_str());
    env[LEN_CGI_ENV] = NULL;
}

void cgi::setCgi(const http::Request &request, const t_locInfos &loc, const t_serverData &dataserv, std::string file)
{
    setCgiMetaVar(request, loc, dataserv, file);
    setCgiEnv();
}

void cgi::Cgi(const http::Request &request, const t_locInfos &loc, const t_serverData &data_serv, std::string file)
{
    int fd_out[2];
    char content[100000];
    int pid;
    std::string root;
    std::string buffer;
    std::string cgi_script;

    int cp_stdin;
    FILE *f_in  =tmpfile();
    int fdin = fileno(f_in);
    cp_stdin = dup(STDIN_FILENO);
 
	(void)request;
    if (pipe(fd_out) == -1)
        std::cout << "PIPE ERROR" << std::endl;

    if (write(fdin, request.header("body").c_str(), request.header("body").size()) == -1)
        std::cerr << "WRITE ERROR :|" << request.header("body") << "| -> could not be written"<< std::endl;
    lseek(fdin, 0, SEEK_SET);

    cgi_script = getenv("CGI_BIN") + SSTR("/") + loc._fastcgiParam;

    char *argv[3];
    argv[0] = strdup(cgi_script.c_str());
    argv[1] = strdup(file.c_str());
    argv[2] = NULL;

    pid = fork();
    if ( pid == 0 )
    {
        if (dup2(fd_out[1], STDOUT_FILENO) == -1)
            std::cerr << "\ndup2 fd_out failed\n" << std::endl;
        if (::close(fd_out[0]) == -1)
            std::cerr << "\nclose fd_out[0] failed in child\n" << std::endl;        

        dup2(fdin, STDIN_FILENO);

        root = (*data_serv.root.rbegin() == '/') ? data_serv.root.substr(0, data_serv.root.size() - 1) : data_serv.root;
        if (execve(argv[0], argv, getCgiEnv()) == -1)
                std::cerr << "EXEC ERROR : " << strerror(errno)  << std::endl;
    }
    else if (pid < 0)
        std::cout << "FORK ERROR" << std::endl;
    else
    {
        waitpid(pid, NULL, -1);
        ::close(fd_out[1]);
        memset(content, 0, sizeof(content));
        _output.clear();
        while (read(fd_out[0], content, sizeof(content)) > 0)
        {
            _output += SSTR(content);
            memset(content, 0, sizeof(content));
        }
        ::close(fd_out[0]);
        memset(content, 0, sizeof(content));
        dup2(STDIN_FILENO, cp_stdin);
        fclose(f_in);
        close(fdin);
        close(cp_stdin);
        free(argv[0]);
        free(argv[1]);
    }
}

std::string cgi::getOutput() const
{
    return _output;
}
