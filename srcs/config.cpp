/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 14:31:53 by alienard          #+#    #+#             */
/*   Updated: 2021/08/31 17:42:37 by pcariou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parsingExceptions.hpp"
#include "webserv.hpp"
#include <cstdlib>
#include <dirent.h>
#include <exception>
#include <iostream>

/******************************************************************************
 *			Constructeurs/Destructeurs
 *****************************************************************************/

config::config()
{
    std::fstream fs;
    fs.open("config.json", std::fstream::in);

    if (!fs.is_open())
    {
        std::cerr << "Error : " << strerror(errno) << std::endl;
        throw ConfigFileException();
    }
    try
    {
        _content = Parser(fs).parse();
        config_check(&_content);
        std::cout << GREEN << "Parsing [OK]" << std::endl;
        std::cout << WHITE << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << RED << e.what() << std::endl;
        std::cerr << "Parsing [KO]" << std::endl;
        exit(EXIT_FAILURE);
    }
    fs.close();
}

/*
 *	Constructeur avec path donné
 */

config::config(char *path)
{
    std::fstream fs;
    if (path)
        fs.open(path, std::fstream::in);
    else
        fs.open("config.json", std::fstream::in);

    if (!fs.is_open())
    {
        std::cerr << "Error : " << strerror(errno) << std::endl;
        throw ConfigFileException();
    }
    try
    {
        _content = Parser(fs).parse();
        config_check(&_content);
        setMimeTypes();
        std::cout << GREEN << "Parsing [OK]" << std::endl;
        std::cout << WHITE << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << RED << e.what() << std::endl;
        std::cerr << "Parsing [KO]" << std::endl;
        exit(EXIT_FAILURE);
    }
    fs.close();
}

/*
 *	Constructeur par copie
 */
config::config(const config &other)
{
    *this = other;
}

/*
 *	Construction par assignation
 */
config &config::operator=(const config &other)
{
    this->server = other.getServer();
    this->port = other.getPort();
    return (*this);
}

config::~config(void)
{
}

std::string config::getServer(void) const
{
    return this->server;
}

int config::getPort(void) const
{
    return this->port;
}

void config::methods_check(std::string &method)
{
    if (method.compare(SSTR("GET")) && method.compare(SSTR("POST")) && method.compare(SSTR("DELETE")))
        throw ValueError::ParsingException("incorrect method value : " + method + ", must be GET, POST, and/or DELETE");
}

void config::fastcgi_param_check(std::pair< const std::string, std::string > &fcgi)
{
    bool found = false;
    DIR *dirp;
    struct dirent *tmp;
    if (fcgi.first.compare(SSTR("fastcgi_param")) == 0)
    {
        std::string cgi_bin;

        cgi_bin = getenv("PWD") + SSTR("/cgi_bin");
        dirp = opendir(cgi_bin.c_str());
        while (dirp && (tmp = readdir(dirp)) != NULL)
        {
            if (strlen(tmp->d_name) == fcgi.second.size() && strcmp(tmp->d_name, fcgi.second.c_str()) == 0)
            {
                (void)closedir(dirp);
                dirp = NULL;
                found = true;
            }
        }
        if (dirp)
            closedir(dirp);
        if (!found)
            throw ValueError::ParsingException("incorrect CGI script value : " + fcgi.second +
                                               ", no corresponding file found");
    }
}

void config::setUploadDir(t_locationData &lD, t_serverData sD)
{
    std::fstream uploadFile;
    std::fstream uploadTemplateFile("upload.php");
    std::string line;

    uploadFile.open(
        (strtrim(sD.root, "/") + "/" + strtrim(lD.path, "/") + ((lD.path != "/") ? "/" : "") + "upload.php").c_str(),
        std::fstream::out);
    while (std::getline(uploadTemplateFile, line))
    {
        uploadFile << line << std::endl;
    }
    uploadFile.close();
    uploadTemplateFile.close();
    mkdir((strtrim(sD.root, "/") + "/" + strtrim(lD.path, "/") + ((lD.path != "/") ? "/" : "") +
           strtrim(lD.upload_dir, "/"))
              .c_str(),
          0777);
}

void config::locationData_check(t_locationData &lD, t_serverData sD)
{
    if (lD.path.empty())
        throw ValueError::ParsingException("incorrect location's path, must be set");

    // checking autoindex
    if (lD.autoindex != true && lD.autoindex != false)
        throw ValueError::ParsingException("incorrect autoindex");

    // set upload directories
    if (!lD.upload_dir.empty())
        config::setUploadDir(lD, sD);

    // checking methods
    for (std::list< std::string >::iterator it = lD.methods.begin(); it != lD.methods.end(); it++)
    {
        config::methods_check(*it);
    }

    for (std::map< std::string, std::string >::iterator it = lD.fastcgi_param.begin(); it != lD.fastcgi_param.end();
         it++)
    {
        config::fastcgi_param_check(*it);
    }
}

void config::error_page_check(std::pair< const int, std::string > &error_page)
{
    std::fstream fs;
    fs.open(error_page.second.c_str(), std::fstream::in);
    if (fs.is_open())
    {
        std::string page((std::istreambuf_iterator< char >(fs)), std::istreambuf_iterator< char >());
        error_page.second = page;
        fs.close();
    }
    else
    {
        // throw ValueError::ParsingException("error_page_not_valid");
        error_page.second = "error_page_not_valid";
    }
}

void config::serverData_check(t_serverData &sD)
{
    Socket sock;
    try
    {
        sock.listen(sD.listen, sD.addr_ip);
        sock.close();
    }
    catch (Socket::SocketException const &e)
    {
        throw ValueError::ParsingException("incorrect listen value : " + SSTR(sD.listen) + " must be available port");
    }
    if (sD.client_max_body_size <= 0)
        throw ValueError::ParsingException("incorrect client_max_body_size : " + SSTR(sD.client_max_body_size) +
                                           " must be > 0");

    for (std::map< int, std::string >::iterator it = sD.error_page.begin(); it != sD.error_page.end(); it++)
        config::error_page_check(*it);

    for (std::list< t_locationData >::iterator it = sD.locations.begin(); it != sD.locations.end(); it++)
    {
        config::locationData_check(*it, sD);
    }
}

void config::config_check(std::list< t_serverData > *_content)
{
    std::list< std::string >::iterator it_name;
    // std::list< t_serverData >::iterator it_listen;
    std::map< std::string, bool > map_name;

    for (std::list< t_serverData >::iterator it = _content->begin(); it != _content->end(); it++)
    {
        /*
           Several servers can listen on the same port
                http://nginx.org/en/docs/http/request_processing.html
         */
        // it_listen = _content->begin();
        // while (it_listen != _content->end())
        // {
        // 	if (it != it_listen && it_listen->listen == it->listen &&
        // it_listen->addr_ip == it->addr_ip) 		throw
        // ValueError::ParsingException("incorrect listen value : two servers are on
        // the same port"); 	it_listen++;
        // }
        // server_name check
        it_name = it->server_name.begin();
        while (it_name != it->server_name.end())
        {
            if (map_name.find(*it_name) != map_name.end())
            {
                throw ValueError::ParsingException("two servers have the save name value : " + *it_name);
            }
            else
            {
                map_name[*it_name] = true;
            }
            it_name++;
        }
        config::serverData_check(*it);
        set(&(*it));
    }
}

void config::set(t_serverData *data)
{
    if (data->root.empty())
        data->root = "root/";
    if (data->index.empty())
        data->index.push_back("index.html");
}

void config::setMimeTypes()
{
    std::ifstream file("mime/mime.types");
    std::string line;
    std::string value;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (!line.empty())
            {
                std::istringstream iss(line);
                std::vector< std::string > result;

                for (std::string s; iss >> s;)
                    result.push_back(s);
                value = result.front();
                for (std::vector< std::string >::iterator it = result.begin(); it != result.end(); ++it)
                {
                    if (*it != *result.begin())
                        _mimeTypes[*it] = value;
                }
                value.clear();
            }
        }
        file.close();
    }
    else
        throw MimeException();
    for (std::list< t_serverData >::iterator it = _content.begin(); it != _content.end(); ++it)
        it->mimeTypes = &_mimeTypes;
}

std::list< t_serverData > config::getContent() const
{
    return this->_content;
}
