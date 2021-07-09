/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 11:08:27 by dess              #+#    #+#             */
/*   Updated: 2021/07/09 20:47:26 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include "request.hpp"
#include "webserv.hpp"
#include <strings.h>

/******************************************************************************
 *			Fonctions statiques permettant l'initialisation d'une
 *socket
 *****************************************************************************/

/*
 * Une fonction qui initialise les informations d'une socket
 *
 * @Parametres: Le nombre de ports et un pointeur sur une structure sockaddr_in
 * @Infos: La structure sockaddr_in est dans le package <netinet/in.h>
 */
static void _initAddress(uint32_t port, struct sockaddr_in *infosPtr, const char *addr_ip)
{
    infosPtr->sin_family = AF_INET;
    infosPtr->sin_port = htons(port);
    infosPtr->sin_addr.s_addr = inet_addr(addr_ip);

    if (infosPtr->sin_addr.s_addr == (uint32_t)-1)
        throw Socket::SocketException();
}

/*
 *	Une fonction qui initialise les options via la fonction setsockopt
 *	@Parametres: Le fd de la socket et un pointeur sur l'option (*opt = 1)
 *	@Infos: La fonction lève une SocketException si setsockopt échoue
 *	@Lien:
 *https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt
 */
static void _initOptions(int fd, int *opt) throw(Socket::SocketException)
{
    // to work on macOS -> suppr SO_REUSEADDR
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(*opt)) < 0)
        throw(Socket::SocketException());
}

/*
 *	Une fonction qui change un fd bloquant en un fd non bloquant via fcntl
 *	@Parametres Le fd
 *	@Infos: La fonction lève une SocketException si fcntl échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/fcntl.2.html
 */
static void _initBlocking(int fd) throw(Socket::SocketException)
{
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
        throw(Socket::SocketException());
}

/*
 *	Une fonction qui permet de lier un fd de socket à un nom via la fonction
 *bind
 *	@Parametres Fd de la socket, les informations de la socket (structure
 *sockaddr_in) et la taille de cette structure
 *	@Infos: La fonction lève une SocketException si bind échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/bind.2.html
 */
static void _initBind(int fd, struct sockaddr_in *address, socklen_t socklen) throw(Socket::SocketException)
{
    if (bind(fd, (struct sockaddr *)address, socklen) < 0)
        throw(Socket::SocketException());
}

/******************************************************************************
 *				Constructeurs et Destructeurs
 *****************************************************************************/

/*
 *	Construit un objet Socket à partir d'un fd.
 *	@Parametres: Un fd et un booleen. Si le booleen est false le fd sera
 *changé en non-bloquant.
 *	@Info: le contructeur utisie getsockname et lève une SocketException si
 *celui-ci échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/getsockname.2.html
 */

Socket::Socket(int fd, bool blocking) throw(Socket::SocketException) : _fd(fd), _opt(1), _socklen(sizeof(_address))
{
    if (getsockname(_fd, (struct sockaddr *)&_address, &_socklen) < 0)
        throw(Socket::SocketException());
    if (!blocking)
        _initBlocking(_fd);
}

/*
 *	Constructeur par défaut de Socket. Seule une socket non initialisée est
 *crée.
 *	@Lien: http://manpagesfr.free.fr/man/man2/socket.2.html
 */
Socket::Socket(void) throw(Socket::SocketException)
    : _fd(socket(AF_INET, SOCK_STREAM, 0)), _opt(1), _socklen(sizeof(_address))
{
    if (_fd < 0)
        throw(Socket::SocketException());
}

/*
 *	Constructeur par copie (deepcopy) de Socket
 *	@Info: Le fd est dupliqué via la fonction dup
 *	@Lien: http://manpagesfr.free.fr/man/man2/dup.2.html
 */
Socket::Socket(const Socket &other) throw(Socket::SocketException)
    : _fd(dup(other._fd)), _opt(other._opt), _address(other._address), _socklen(other._socklen)
{
}

Socket::~Socket(void)
{
}

/******************************************************************************
 *					SocketException
 *****************************************************************************/
/*
 *	Le message correspond au message de errno
 */
const char *Socket::SocketException::what() const throw()
{
    return strerror(errno);
}

/******************************************************************************
 *								Getters
 *****************************************************************************/

/*
 *	Retourne le fd de la socket
 */
int Socket::Fd() const
{
    return _fd;
}

/*
 *	Retourne la request de la socket
 */
std::string Socket::get_request() const
{
    return _request;
}

/*
 *	Retourne la request parsée de la socket
 */
http::Request Socket::get_m_request(void) const
{
    return m_request;
}

/*
 *	Retourne les infos de la socket (structure sockaddr_in)
 */
struct sockaddr_in Socket::infos() const
{
    return _address;
}

t_locationData *Socket::get_locationData(void) const
{
    return _loc;
}

std::vector< std::string > Socket::get_infos(void) const
{
    return _infos;
}

/******************************************************************************
 *							Fonction membres
 *****************************************************************************/

/*
 *	Mets la socket en écoute sur un port spécifique
 *	@Parametres: Le port
 *	@Infos: Lève une SocketException si erreur
 */
void Socket::listen(const int port, const std::string addr) throw(Socket::SocketException)
{
    _initAddress(port, &_address, addr.c_str());
    _initOptions(_fd, &_opt);
    _initBind(_fd, &_address, _socklen);
    _initBlocking(_fd);

    if (::listen(_fd, MAX_CONN) < 0)
        throw(Socket::SocketException());
}

/*
 *	Accept la connection avec un client et retourne la socket client
 *	@Infos: Le fd de la socket client retournée est non-bloquant.
 *	La fonction lève une SocketException si erreur
 */
Socket Socket::accept(void) throw(Socket::SocketException)
{
    int clientSocket = ::accept(_fd, (struct sockaddr *)&_address, &_socklen);
    if (clientSocket < 0)
        throw(Socket::SocketException());
    // store buf in words table (infos)
    return Socket(clientSocket, false);
}

/*
 *	Lit une partie du contenu reçu par la socket
 *	@Infos: La fonction lève une SocketException si erreur
 */
std::string Socket::readContent(void) throw(Socket::SocketException)
{
    int ret = 0;
    char buffer[300];

    bzero(buffer, sizeof(buffer));
    ret = recv(_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
    return std::string(static_cast< char * >(buffer), ret);
}

/*
 *	Envoie un contenu via la socket
 *	@Infos: La fonction lève une SocketException si erreur
 */
void Socket::send(const std::string content) throw(SocketException)
{
    if (::send(_fd, content.c_str(), content.size(), 0) == -1)
        throw Socket::SocketException();
}

/*
 * Generic function to find if an element of any type exists in list
 */
template < typename T >

bool contains(std::list< T > &listOfElements, const T &element)
{
    // Find the iterator if element in li st
    typename std::list< T >::iterator it = std::find(listOfElements.begin(), listOfElements.end(), element);
    // return if iterator points to end or not. It points to end then it means
    // element does not exists in list
    return it != listOfElements.end();
}

void Socket::headerCode(std::string content, int code, t_serverData data)
{
    if (!data.error_page[code].empty() && data.error_page[code] == "error_page_not_valid" && code != 404)
    {
        !data.error_page[404].empty() ? _content = data.error_page[404] : _content = "<h1>" + content + "</h1>";
        _code = "404 Not Found";
        return;
    }
    if (!data.error_page[code].empty() && data.error_page[code] == "error_page_not_valid")
    {
        _content = "<h1>" + content + "</h1>";
        _code = content;
        return;
    }
    !data.error_page[code].empty() ? _content = data.error_page[code] : _content = "<h1>" + content + "</h1>";
    _code = content;
}

void Socket::sendpage(t_serverData data)
{
    std::ostringstream oss;
    time_t now = time(0);
    char *dt = ctime(&now);

    (void)data;
    oss << "HTTP/1.1 " << _code << "\r\n";
    oss << "Server: Webserv"
        << "\r\n";
    oss << "Date: " << dt;
    oss << "Content-Length: " << _content.size() << "\r\n";
    if (_code == "301 Moved Permanently")
    {
        oss << "Location: http://" + m_request.header("Host") + _infos[1] + "/";
        if (_isDir)
            oss << _index;
        oss << "\r\n";
    }
    oss << "Connection: ";
    std::string connect = (_code == "400 Bad Request") ? "close\r\n" : "keep-alive\r\n";
    oss << connect;
    if (!php_file() || _code != "200 OK")
        oss << "\r\n";
    oss << _content;
    ::send(_fd, oss.str().c_str(), oss.str().size(), 0);
    std::cout << "		-- SERVER RESPONSE --\n\n" << oss.str().c_str();
}

void Socket::directoryListing(std::string file, t_serverData data)
{
    DIR *dh;
    DIR *is_dir;
    struct dirent *contents;
    std::string directory = file;
    std::fstream f;
    std::string d_slash;

    if (!(dh = opendir(directory.c_str())))
        headerCode("404 Not Found", 404, data);
    else
    {
        _content += ("<h1>Index of " + _infos[1] + "</h1>\n");
        while ((contents = readdir(dh)) != NULL)
        {
            if ((is_dir = opendir((data.root + _infos[1] + std::string(contents->d_name)).c_str())))
                d_slash = "/";
            closedir(is_dir);
            if (std::string(contents->d_name) != ".")
                _content += ("<li><a href=\"" + std::string(contents->d_name) + d_slash + "\">" +
                             (std::string(contents->d_name) + d_slash + "</a></li>\n"));
            d_slash = "";
        }
    }
    if (directory[directory.size() - 1] != '/')
        headerCode("301 Moved Permanently", 301, data);
    closedir(dh);
}

// check extension of a file (.php)
bool Socket::php_file()
{
    std::string ext;

    for (std::string::reverse_iterator it = _infos[1].rbegin(); it != _infos[1].rend(); ++it)
    {
        if (*it == '.')
            break;
        ext += *it;
    }
    if (ext == "php")
        return true;
    return false;
}

std::string Socket::Cgi(t_serverData &data)
{
    int fd[2];
    char content[100000];
    int pid;

    // setCgiEnv(data);
    cgi cgi_data(*this, data);
    // for (int i = 0; cgi_data.getCgiEnv()[i]; i++){
    //	std::cout << "env["<<i<<"] = |"<<cgi_data.getCgiEnv()[i] << "|"<< std::endl;
    //}
    pipe(fd);
    if ((pid = fork()) == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        ::close(fd[0]);
        ::close(fd[1]);
        // execl("cgi-bin/php-cgi7.0", "cgi-bin/php-cgi7.0", ("www" + _infos[1]).c_str(), NULL);
        execle("cgi-bin/php-cgi7.0", "cgi-bin/php-cgi7.0", ("www" + _infos[1]).c_str(), cgi_data.getCgiEnv(), NULL);
        // execl("php-cgi", "php-cgi", ("www" + _infos[1]).c_str(), NULL);
    }
    ::close(fd[1]);
    read(fd[0], content, sizeof(content));
    ::close(fd[0]);
    waitpid(pid, NULL, -1);
    return (std::string(content));
}

void Socket::badRequest(t_serverData data)
{
    headerCode("400 Bad Request", 400, data);
    sendpage(data);
}

void Socket::Delete(t_serverData data)
{
    std::ostringstream oss;
    _code = "200 OK";

    std::ifstream f((data.root + _infos[1]).c_str());
    bool isDir = (f.good() && !f.rdbuf()->in_avail()) ? true : false;
    f.close();

    if (isDir)
        headerCode("409 Conflict", 409, data);
    else if (!remove((data.root + _infos[1]).c_str()))
        _content = "<h1>" + _infos[1] + " deleted</h1>";
    else if (errno != 2)
        headerCode("403 Forbidden", 403, data);
    else
        headerCode("404 Not Found", 404, data);
    sendpage(data);
}

void Socket::Post(t_serverData data)
{
    _code = "200 OK";
    std::ofstream f("www/post");
    (void)data;

    f << m_request.header("Body");
    _infos[1] = "/post";
    //_content = Cgi(data);
    f.close();
    // sendpage(data);
}

void Socket::Get(t_serverData data)
{
    std::fstream f;
    std::string file;
    _code = "200 OK";

    if (_infos[1] == "/" || (_directory && !_index.empty() && !data.autoindex)) // directory request
        file = data.root + _infos[1] + _index;
    else
        file = data.root + _infos[1]; // classic path request
    f.open(file.c_str(), std::ios::in);
    if ((f.good() && !f.rdbuf()->in_avail()) ||
        (!f.good() && !access(file.c_str(), F_OK))) // if directory or file with no rights
    {
        if ((f.good() && !f.rdbuf()->in_avail()) && data.autoindex)
            directoryListing(file, data);
        else if (f.good() && !f.rdbuf()->in_avail() && !_index.empty())
            headerCode("301 Moved Permanently", 301, data);
        else
            headerCode("403 Forbidden", 403, data);
    }
    else if (f.good() && php_file()) // if .php
        _content = Cgi(data);
    else if (f.good())
        _content = std::string((std::istreambuf_iterator< char >(f)), std::istreambuf_iterator< char >());
    else if (_directory && !_index.empty() && !data.autoindex)
        headerCode("403 Forbidden", 403, data);
    else
        headerCode("404 Not Found", 404, data);
    f.close();
    sendpage(data);
}

// set autoindex true if autoindex is activated in a parent's location
void Socket::locAutoindex(t_serverData *data)
{
    std::string path1;

    for (std::list< t_locationData >::iterator it = data->locations.begin(); it != data->locations.end(); ++it)
    {
        path1 = (it->path[it->path.size() - 1] == '/') ? it->path.substr(0, it->path.size() - 1) : it->path;
        if (it->autoindex && _infos[1].find(path1) != std::string::npos)
            data->autoindex = true;
    }
}

// check if path/
void Socket::directory(const std::string &name)
{
    _directory = (*name.rbegin() == '/') ? true : false;
}

bool Socket::fileExists(t_serverData data, const std::string &name)
{
    bool exists;

    std::ifstream f((data.root + _infos[1] + "/" + name).c_str());
    exists = f.good();
    f.close();
    return exists;
}

// check which file to use in index
// set index if set in a parent's location
void Socket::locIndex(t_serverData data)
{
    std::string path1;

    for (std::list< std::string >::iterator it = data.index.begin(); it != data.index.end(); ++it)
    {
        if (!it->empty() && fileExists(data, *it))
        {
            _index = *it;
            break;
        }
    }
    for (std::list< t_locationData >::iterator it1 = data.locations.begin(); it1 != data.locations.end(); ++it1)
    {
        path1 = (it1->path[it1->path.size() - 1] == '/') ? it1->path.substr(0, it1->path.size() - 1) : it1->path;
        if (!it1->index.front().empty() && _infos[1].find(path1) != std::string::npos)
        {
            for (std::list< std::string >::iterator it2 = it1->index.begin(); it2 != it1->index.end(); ++it2)
            {
                if (!it2->empty() && fileExists(data, *it2))
                {
                    _index = *it2;
                    break;
                }
            }
        }
    }
    std::ifstream f((data.root + _infos[1] + "/" + _index).c_str());
    _isDir = (f.good() && !f.rdbuf()->in_avail()) ? true : false;
    f.close();
}

// return true if method is not allowed in a parent's location
bool Socket::methodAllowed(t_serverData data)
{
    std::string path1;

    for (std::list< t_locationData >::iterator it = data.locations.begin(); it != data.locations.end(); ++it)
    {
        path1 = (it->path[it->path.size() - 1] == '/') ? it->path.substr(0, it->path.size() - 1) : it->path;
        if (_infos[1].find(path1) != std::string::npos && contains(it->methods, _infos[0]))
            return false;
    }
    return true;
}

void Socket::serverResponse(t_serverData data)
{
    directory(_infos[1]);
    locIndex(data);
    locAutoindex(&data);
    if (_infos.size() >= 3 && _infos[2] == "HTTP/1.1" && !m_request.header("Host").empty())
    {
        if (_infos[0] == "GET" && methodAllowed(data))
            Get(data);
        else if (_infos[0] == "POST" && methodAllowed(data))
            Post(data);
        else if (_infos[0] == "DELETE" && methodAllowed(data))
            Delete(data);
        else
        {
            headerCode("405 Method Not Allowed", 405, data);
            sendpage(data);
        }
    }
    else
        badRequest(data);
    _request.clear();
    _content.clear();
}

/*
 *	Ferme le fd à l'aide la de fonction close
 *	@Infos: Doit être appelée avant la fermeture du programme
 */
void Socket::close(void)
{
    ::close(_fd);
}

/******************************************************************************
 *			Operator overloading
 *****************************************************************************/

Socket &Socket::operator=(const Socket &other)
{
    _fd = other._fd;
    _opt = other._opt;
    _address = other._address;
    _socklen = other._socklen;
    return *this;
}

bool Socket::operator==(const int fd) const
{
    return fd == _fd;
}

bool Socket::operator==(const Socket &other) const
{
    return _fd == other._fd;
}

bool operator==(const int fd, const Socket &s)
{
    return fd == s.Fd();
}
