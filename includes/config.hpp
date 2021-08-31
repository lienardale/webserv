#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "parsing/parser.hpp"
#include <iostream>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>

class config
{
  public:
    class ConfigFileException : public std::exception
    {
      public:
        ConfigFileException(void) throw()
        {
            return;
        }
        ConfigFileException(const ConfigFileException &src) throw();
        virtual ~ConfigFileException(void) throw()
        {
            return;
        }
        ConfigFileException &operator=(const ConfigFileException &rhs) throw();
        virtual const char *what() const throw()
        {
            return ("Error: Invalid config file (webserv.conf)");
        }
    };
	class MimeException : public std::exception
    {
      public:
        MimeException(void) throw()
        {
            return;
        }
        MimeException(const MimeException &src) throw();
        virtual ~MimeException(void) throw()
        {
            return;
        }
        MimeException &operator=(const MimeException &rhs) throw();
        virtual const char *what() const throw()
        {
            return ("Error: Invalid mime types file (mime/mime.types)");
        }
    };
    config(void);
    config(char *path);
    config(std::string server, int port);
    config(const config &src);
    ~config(void);
    config &operator=(const config &rhs);
    std::string getServer(void) const;
    int getPort(void) const;
    void config_check(std::list< t_serverData > *_content) throw(ParsingException);
    void serverData_check(t_serverData &sD);
    void set(t_serverData *data);
    int sD_index_check(const char *dir, std::string &index);
    void server_name_check(std::string &server_name);
    void error_page_check(std::pair< const int, std::string > &error_page);
    void locationData_check(t_locationData &lD, t_serverData sD);
    int lD_index_check(const char *dir, std::string &index);
    void fastcgi_param_check(std::pair< const std::string, std::string > &fcgi, const char *root);
    void methods_check(std::string &method);
    std::list< t_serverData > getContent() const;
	void setMimeTypes();
	void setUploadDir(t_locationData &lD, t_serverData sD);
    // int ft_print_config(void);

  private:
    std::string server;
    int port;
    std::list< t_serverData > _content;
	std::map<std::string, std::string> _mimeTypes;
};

#endif
