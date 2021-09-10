/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleDELETE.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 18:50:25 by dboyer            #+#    #+#             */
/*   Updated: 2021/09/08 12:07:37 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "webserv.hpp"

http::Response handleDELETE(const http::Request &request, const t_serverData &data)
{
    std::string path = data.root.substr(0, data.root.size() - 1) + request.header("path");
    std::ifstream f(path.c_str());
    bool isDir = f.good() && !f.rdbuf()->in_avail();
    f.close();

    if (isDir)
        return http::Response(http::CONFLICT);

    int ret = remove(path.c_str());
    if (ret == 0)
        return http::Response(http::OK);
    if (errno == 2)
        return http::Response(http::NOT_FOUND);
    return http::Response(http::FORBIDDEN);
}
