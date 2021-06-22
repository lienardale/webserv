/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 09:31:19 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/22 14:07:00 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "parsing/dataStructure.hpp"
#include "socket.hpp"
#include <cstddef>
#include <iostream>
#include <list>
#include <map>
#include <sys/epoll.h>
#include <utility>

#define MAX_EVENTS 5

/****************************************************************************************
 *				Outils
 ***************************************************************************************/
static void _add_server_to_poll( int epoll_fd, int socket_fd ) throw( Socket::SocketException )
{
	struct epoll_event event;

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = socket_fd;
	if ( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, socket_fd, &event ) )
	{
		close( epoll_fd );
		throw Socket::SocketException();
	}
}
/******************************************************************************
 *			Constructeurs/Destructeurs
 *****************************************************************************/

/*
 *	Initialise un serveur qui écoutera sur un port donnée
 *	@Parametres: Le port qui sera écouté et l'interval (en seconde) entre chaque écoute
 *	@Lien: http://manpagesfr.free.fr/man/man2/select.2.html
 */
http::Server::Server() : _run( false ), _epoll_fd( epoll_create( 1 ) )
{
}

/*
 *	Constructeur par copie
 */
http::Server::Server( const Server &other )
	: _timeout( other._timeout ), _configs( other._configs ), _serverSet( other._serverSet ), _run( other._run ),
	  _epoll_fd( other._epoll_fd )
{
}

/*
 *	Construction par assignation
 */
http::Server &http::Server::operator=( const Server &other )
{
	_configs = other._configs;
	_serverSet = other._serverSet;
	_run = other._run;
	return *this;
}

http::Server::~Server( void )
{
}

/****************************************************************************************
 *			Fonctions membres
 ***************************************************************************************/

void http::Server::init( const std::list< t_serverData > &configs, uint32_t timeout )
{
	_timeout.tv_sec = timeout;
	_timeout.tv_usec = 0;
	_configs = configs;
}

/*
 *	Fonction qui va mettre le serveur en écoute sur le port spécifié dans le constructeur
 *	@Infos: la fonction arrête le programme si une SocketException est levé
 */
void http::Server::listen( void )
{
	try
	{
		for ( std::list< t_serverData >::iterator it = _configs.begin(); it != _configs.end(); it++ )
		{
			Socket s;
			s.listen( it->listen, it->addr_ip );
			_serverSet[ s.Fd() ] = std::make_pair( s, *it );
		}
		_watchFds();
	}
	catch ( Socket::SocketException &e )
	{
		std::cerr << e.what() << std::endl;
		exit( EXIT_FAILURE );
	}
}

/*
 *	Fonction qui se charge de la lecture des données reçues par le serveur via la socket client
 *	@Parametres: Le fd sur lequel la lecture doit se faire
 *	@Infos La fonction lève une SocketException si erreur
 */
void http::Server::_handleReady( int epoll_fd, const int fd ) throw( Socket::SocketException )
{
	std::map< int, std::pair< Socket, t_serverData > >::iterator found = _serverSet.find( fd );

	if ( found != _serverSet.end() )
	{
		// this is a new connection
		try
		{
			_add_server_to_poll( epoll_fd, found->second.first.accept().Fd() );
			_currentData = found->second.second;
		}
		catch ( Socket::SocketException &e )
		{
			std::cerr << e.what() << std::endl;
		}
	}
	else
	{
		try
		{
			_currentSock = Socket( fd, true );
			_currentSock.readContent();
			_currentSock.serverResponse( _currentData );
			_currentSock.close();
		}
		catch ( Socket::SocketException &e )
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

/*
 *	C'est la boucle principale du serveur.Avant chaque lecture/écriture il y a un select()
 *	@Infos: La fonction lève une SocketException si erreur
 *	@Lien: http://manpagesfr.free.fr/man/man2/select.2.html
 */
void http::Server::_watchFds( void ) throw( Socket::SocketException )
{
	struct epoll_event events[ MAX_EVENTS ];
	int epoll_fd = epoll_create( 1 );
	int event_count = 0;

	_run = true;
	for ( std::map< int, std::pair< Socket, t_serverData > >::iterator it = _serverSet.begin(); it != _serverSet.end();
		  it++ )
		_add_server_to_poll( epoll_fd, it->first );

	while ( _run )
	{
		event_count = epoll_wait( epoll_fd, events, MAX_EVENTS, -1 );
		for ( int i = 0; i < event_count; i++ )
			_handleReady( epoll_fd, events[ i ].data.fd );
	}
}

/*
 *	Fonction qui stop le serveur et ferme tous les fd ouverts
 */
void http::Server::stop( void )
{
	std::cout << "Gracefully stopping the server..." << std::endl;
	if ( _run )
	{
		_run = false;
		for ( size_t i = 0; i < _serverSet.size() + 1; i++ )
			Socket( i, true ).close();
	}
}
