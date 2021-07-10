/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/07/10 15:48:35 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "webserv.hpp"

static std::string cleanPath(const std::string &root, const std::string &path)
{
    if (root[root.size() - 1] != '/')
        return root + "/" + (++path.begin()).base();

    return root + (++path.begin()).base();
}

http::Response handleDELETE(const http::Request &request, const t_locationData &data)
{
    std::string path = cleanPath(data.root, request.header("path"));
    std::ifstream f(path.c_str());
    bool isDir = f.good() && !f.rdbuf()->in_avail();
    f.close();

    if (isDir)
        return http::Response(http::CONFLICT);

    int ret = remove(path.c_str());
    std::cout << "ret = " << ret << " path = " << path << std::endl;
    if (ret == 0)
        return http::Response(http::OK);
    if (errno == 2)
        return http::Response(http::NOT_FOUND);
    return http::Response(http::FORBIDDEN);
}