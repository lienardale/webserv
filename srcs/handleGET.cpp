/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGET.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/12 18:27:52 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "webserv.hpp"
#include "cgi.hpp"

std::string Cgi(const http::Request &request, const t_locationData &data)
{
    int fd[2];
    char content[100000];
    int pid;

    cgi cgi_data(request, data);
    pipe(fd);
    if ((pid = fork()) == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        ::close(fd[0]);
        ::close(fd[1]);
        execle("cgi-bin/php-cgi7.0", "cgi-bin/php-cgi7.0", ("www" + request.header("Path")).c_str(), cgi_data.getCgiEnv(), NULL);
    }
    ::close(fd[1]);
    read(fd[0], content, sizeof(content));
    ::close(fd[0]);
    waitpid(pid, NULL, -1);
    return (std::string(content));
}

void directoryListing(std::string file, const t_locationData &data, std::string _content, http::Response &ret, const http::Request &request)
{
    DIR *dh;
    DIR *is_dir;
    struct dirent *contents;
    std::string directory = file;
    std::fstream f;
    std::string d_slash;

    if (!(dh = opendir(directory.c_str()))){
        ret.setCode(http::NOT_FOUND);
        std::cout << "IT IS THIS SHIT THAT IS NOT WORKING" << std::endl;
    }
    else
    {
        _content += ("<h1>Index of " + request.header("Path") + "</h1>\n");
        while ((contents = readdir(dh)) != NULL)
        {
            if ((is_dir = opendir((data.root + request.header("Path") + std::string(contents->d_name)).c_str())))
                d_slash = "/";
            closedir(is_dir);
            if (std::string(contents->d_name) != ".")
                _content += ("<li><a href=\"" + std::string(contents->d_name) + d_slash + "\">" +
                             (std::string(contents->d_name) + d_slash + "</a></li>\n"));
            d_slash = "";
        }
    }
    if (directory[directory.size() - 1] != '/')
        ret.setCode(http::MOVED_PERMANENTLY);
    closedir(dh);
}

bool php_file(std::string file)
{
    std::string ext;

    for (std::string::reverse_iterator it = file.rbegin(); it != file.rend(); ++it)
    {
        if (*it == '.')
            break;
        ext += *it;
    }
    if (ext == "php")
        return true;
    return false;
}



http::Response handleGET(const http::Request &request, const t_locationData &data, t_dirinfo *dir_info)
{
    // (void)request;
    // (void)data;
    std::fstream f;
    std::string file;
    std::string _content;

    http::Response ret = http::Response(http::OK);

    // ret.setHeader();

    std::cout << "IN HANDLE GET" << std::endl;

    file = data.root + request.header("Path");
    if (request.header("Path") == "/" || (dir_info->_directory && !dir_info->_index.empty() && !data.autoindex)) //directory to set somewhere
        file += dir_info->_index;
    f.open(file.c_str(), std::ios::in);

    if ((f.good() && !f.rdbuf()->in_avail()) ||
        (!f.good() && !access(file.c_str(), F_OK)))
    {
        if ((f.good() && !f.rdbuf()->in_avail()) && data.autoindex)
            directoryListing(file, data, _content, ret, request); // to modify and/or move
        else if (f.good() && !f.rdbuf()->in_avail() && !dir_info->_index.empty())
            ret.setCode(http::MOVED_PERMANENTLY);
        else
            ret.setCode(http::FORBIDDEN);
    }
    else if (f.good() && php_file(request.header("Path")))
        ret.setBody(Cgi(request, data), "text/html"); // Cgi fct to modify and/or move
    else if (f.good())
        ret.setBody(std::string((std::istreambuf_iterator< char >(f)), std::istreambuf_iterator< char >()), "text/html");
    else if (dir_info->_directory && !dir_info->_index.empty() && !data.autoindex)
        ret.setCode(http::FORBIDDEN);
    else
        ret.setCode(http::NOT_FOUND);
    f.close();
    // ret.setBody(request.header("method") + " " + request.header("path") + " " + request.header("protocol"),
    //             "text/html; charset=utf-8");
    return ret;
}
