/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 11:08:27 by dess              #+#    #+#             */
/*   Updated: 2021/06/21 21:10:04 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket.hpp"
#include <inttypes.h>
#include <netinet/in.h>

/******************************************************************************
 *			Fonctions statiques permettant l'initialisation d'une socket
 *****************************************************************************/

/*
 * Une fonction qui initialise les informations d'une socket
 *
 * @Parametres: Le nombre de ports et un pointeur sur une structure sockaddr_in
 * @Infos: La structure sockaddr_in est dans le package <netinet/in.h>
 */
static void _initAddress( uint32_t port, struct sockaddr_in *infosPtr, const char *addr_ip )
{
	infosPtr->sin_family = AF_INET;
	infosPtr->sin_port = htons( port );
	infosPtr->sin_addr.s_addr = inet_addr( addr_ip );

	if ( infosPtr->sin_addr.s_addr == ( uint32_t )-1 )
		throw Socket::SocketException();
}

/*
 *	Une fonction qui initialise les options via la fonction setsockopt
 *	@Parametres: Le fd de la socket et un pointeur sur l'option (*opt = 1)
 *	@Infos: La fonction lève une SocketException si setsockopt échoue
 *	@Lien: https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt
 */
static void _initOptions( int fd, int *opt ) throw( Socket::SocketException )
{
	if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof( *opt ) ) < 0 )
		throw( Socket::SocketException() );
}

/*
 *	Une fonction qui change un fd bloquant en un fd non bloquant via fcntl
 *	@Parametres Le fd
 *	@Infos: La fonction lève une SocketException si fcntl échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/fcntl.2.html
 */
static void _initBlocking( int fd ) throw( Socket::SocketException )
{
	if ( fcntl( fd, F_SETFL, O_NONBLOCK ) < 0 )
		throw( Socket::SocketException() );
}

/*
 *	Une fonction qui permet de lier un fd de socket à un nom via la fonction bind
 *	@Parametres Fd de la socket, les informations de la socket (structure sockaddr_in) et la taille de cette structure
 *	@Infos: La fonction lève une SocketException si bind échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/bind.2.html
 */
static void _initBind( int fd, struct sockaddr_in *address, socklen_t socklen ) throw( Socket::SocketException )
{
	if ( bind( fd, ( struct sockaddr * )address, socklen ) < 0 )
		throw( Socket::SocketException() );
}

/******************************************************************************
 *				Constructeurs et Destructeurs
 *****************************************************************************/

/*
 *	Construit un objet Socket à partir d'un fd.
 *	@Parametres: Un fd et un booleen. Si le booleen est false le fd sera changé en non-bloquant.
 *	@Info: le contructeur utisie getsockname et lève une SocketException si celui-ci échoue
 *	@Lien: http://manpagesfr.free.fr/man/man2/getsockname.2.html
 */
Socket::Socket( int fd, bool blocking ) throw( Socket::SocketException )
	: _fd( fd ), _opt( 1 ), _socklen( sizeof( _address ) )
{
	if ( getsockname( _fd, ( struct sockaddr * )&_address, &_socklen ) < 0 )
		throw( Socket::SocketException() );
	if ( !blocking )
		_initBlocking( _fd );
}

/*
 *	Constructeur par défaut de Socket. Seule une socket non initialisée est crée.
 *	@Lien: http://manpagesfr.free.fr/man/man2/socket.2.html
 */
Socket::Socket( void ) throw( Socket::SocketException )
	: _fd( socket( AF_INET, SOCK_STREAM, 0 ) ), _opt( 1 ), _socklen( sizeof( _address ) )
{
	if ( _fd < 0 )
		throw( Socket::SocketException() );
}

/*
 *	Constructeur par copie (deepcopy) de Socket
 *	@Info: Le fd est dupliqué via la fonction dup
 *	@Lien: http://manpagesfr.free.fr/man/man2/dup.2.html
 */
Socket::Socket( const Socket &other ) throw( Socket::SocketException )
	: _fd( dup( other._fd ) ), _opt( other._opt ), _address( other._address ), _socklen( other._socklen )
{
}

Socket::~Socket( void )
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
	return strerror( errno );
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
 *	Retourne les infos de la socket (structure sockaddr_in)
 */
struct sockaddr_in Socket::infos() const
{
	return _address;
}

/******************************************************************************
 *							Fonction membres
 *****************************************************************************/

/*
 *	Mets la socket en écoute sur un port spécifique
 *	@Parametres: Le port
 *	@Infos: Lève une SocketException si erreur
 */
void Socket::listen( const int port, const std::string addr ) throw( Socket::SocketException )
{
	_initAddress( port, &_address, addr.c_str() );
	_initOptions( _fd, &_opt );
	_initBind( _fd, &_address, _socklen );
	_initBlocking( _fd );

	if ( ::listen( _fd, MAX_CONN ) < 0 )
		throw( Socket::SocketException() );
}

/*
 *	Accept la connection avec un client et retourne la socket client
 *	@Infos: Le fd de la socket client retournée est non-bloquant.
 *	La fonction lève une SocketException si erreur
 */
Socket Socket::accept( void ) throw( Socket::SocketException )
{
	int clientSocket = ::accept( _fd, ( struct sockaddr * )&_address, &_socklen );
	if ( clientSocket < 0 )
		throw( Socket::SocketException() );
	// store buf in words table (infos)
	return Socket( clientSocket, false );
}

/*
 *	Lis la totalité du contenu reçu par la socket
 *	@Infos: La fonction lève une SocketException si erreur
 */
void Socket::readContent( void ) throw( Socket::SocketException )
{
	int ret = 0;
	while ( ( ret = recv( _fd, _buffer, sizeof( _buffer ), MSG_DONTWAIT ) > 0 ) )
		_request.append( _buffer );
	if ( ret < 0 )
		throw( Socket::SocketException() );
	std::cout << "		-- CLIENT REQUEST --\n\n" << _request << "\n" << std::endl;

	// transform result in words table (_infos)
	std::istringstream iss( _request );
	_infos = std::vector< std::string >( ( std::istream_iterator< std::string >( iss ) ),
										 std::istream_iterator< std::string >() );
}

/*
 *	Send the requested page to the client
 *
 */

// check extension of a file (.php)
bool Socket::php_file()
{
	std::string ext;

	for ( std::string::reverse_iterator it = _infos[ 1 ].rbegin(); it != _infos[ 1 ].rend(); ++it )
	{
		if ( *it == '.' )
			break;
		ext += *it;
	}
	if ( ext == "php" )
		return true;
	return false;
}

std::string Socket::Cgi()
{
	int fd[ 2 ];
	char content[ 100000 ];

	pipe( fd );
	if ( fork() == 0 )
	{
		dup2( fd[ 1 ], STDOUT_FILENO );
		::close( fd[ 0 ] );
		::close( fd[ 1 ] );
		execl( "php-cgi", "php-cgi", ( "www" + _infos[ 1 ] ).c_str(), NULL );
	}
	::close( fd[ 1 ] );
	read( fd[ 0 ], content, sizeof( content ) );
	::close( fd[ 1 ] );
	wait( NULL );
	return ( std::string( content ) );
}

void Socket::badRequest( void )
{
	std::string content = "<h1>400 Bad Request</h1>";
	std::ostringstream oss;

	oss << "HTTP/1.1 400 Bad Request\r\n";
	oss << content;

	send( _fd, oss.str().c_str(), oss.str().size(), 0 );
	std::cout << "		-- SERVER RESPONSE --\n\n" << oss.str().c_str() << "\n" << std::endl;
}

void Socket::Delete( void )
{
	std::string content = "<h1>404 Not Found</h1>";
	std::ostringstream oss;
	std::string code = "404 Not Found";

	if ( !remove( ( "www" + _infos[ 1 ] ).c_str() ) )
	{
		code = "200 OK";
		content = "<h1>" + _infos[ 1 ] + " deleted</h1>";
	}
	else if ( errno != 2 )
	{
		code = "403 Forbidden";
		content = "<h1>403 Forbidden</h1>";
	}
	oss << "HTTP/1.1 ";
	oss << code;
	oss << "\r\n";
	oss << content;

	send( _fd, oss.str().c_str(), oss.str().size(), 0 );
	std::cout << "		-- SERVER RESPONSE --\n\n" << oss.str().c_str() << "\n" << std::endl;
}

void Socket::Post( void )
{
	std::cout << "REQUEST" << _request << std::endl;
	//	send(_fd, oss.str().c_str(), oss.str().size(), 0);
	//	std::cout << "		-- SERVER RESPONSE --\n\n" << oss.str().c_str() << "\n" << std::endl;
}

void Socket::Get( void )
{
	std::string content = "<h1>404 Not Found</h1>";
	std::string code = "200 OK";
	std::fstream f;

	if ( _infos.size() >= 3 && _infos[ 0 ] == "GET" )
	{
		if ( _infos[ 1 ] == "/" )
			f.open( "www/index.html" );
		else
			f.open( ( "www" + _infos[ 1 ] ).c_str(), std::ios::in );
		if ( f && php_file() )
			content = Cgi();
		else if ( f )
		{
			std::string page( ( std::istreambuf_iterator< char >( f ) ), std::istreambuf_iterator< char >() );
			content = page;
		}
		else
			code = "404 Not Found";
		f.close();
	}
	std::ostringstream oss;
	oss << "HTTP/1.1 ";
	oss << code;
	oss << "\r\n";
	oss << "Server: WEBSERV\r\n";
	oss << "Content-Length: " << content.size() << "\r\n";
	if ( !php_file() )
		oss << "\r\n";
	oss << content;
	send( _fd, oss.str().c_str(), oss.str().size(), 0 );
	std::cout << "		-- SERVER RESPONSE --\n\n" << oss.str().c_str() << "\n" << std::endl;
}

void Socket::serverResponse( void )
{
	if ( _infos.size() >= 3 && _infos[ 2 ] == "HTTP/1.1" )
	{
		if ( _infos[ 0 ] == "GET" )
			Get();
		else if ( _infos[ 0 ] == "POST" )
			Post();
		else if ( _infos[ 0 ] == "DELETE" )
			Delete();
	}
	else
		badRequest();
	_request.clear();
}

/*
 *	Ferme le fd à l'aide la de fonction close
 *	@Infos: Doit être appelée avant la fermeture du programme
 */
void Socket::close( void )
{
	::close( _fd );
}

/******************************************************************************
 *			Operator overloading
 *****************************************************************************/

Socket &Socket::operator=( const Socket &other )
{
	_fd = other._fd;
	_opt = other._opt;
	_address = other._address;
	_socklen = other._socklen;
	return *this;
}

bool Socket::operator==( const int fd ) const
{
	return fd == _fd;
}

bool Socket::operator==( const Socket &other ) const
{
	return _fd == other._fd;
}

bool operator==( const int fd, const Socket &s )
{
	return fd == s.Fd();
}
