/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboyer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 09:31:19 by dboyer            #+#    #+#             */
/*   Updated: 2021/06/22 09:31:25 by dboyer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "parsing/dataStructure.hpp"
#include "socket.hpp"
#include <list>
#include <utility>

/******************************************************************************
 *			Constructeurs/Destructeurs
 *****************************************************************************/

/*
 *	Initialise un serveur qui écoutera sur un port donnée
 *	@Parametres: Le port qui sera écouté et l'interval (en seconde) entre chaque écoute
 *	@Lien: http://manpagesfr.free.fr/man/man2/select.2.html
 */
http::Server::Server() : _run( false )
{

	FD_ZERO( &_readSet );
	FD_ZERO( &_writeSet );
	FD_ZERO( &_exceptSet );
}

/*
 *	Constructeur par copie
 */
http::Server::Server( const Server &other )
	: _timeout( other._timeout ), _configs( other._configs ), _serverSet( other._serverSet ),
	  _readSet( other._readSet ), _writeSet( other._writeSet ), _exceptSet( other._exceptSet ), _run( other._run )
{
}

/*
 *	Construction par assignation
 */
http::Server &http::Server::operator=( const Server &other )
{
	_configs = other._configs;
	_serverSet = other._serverSet;
	_readSet = other._readSet;
	_writeSet = other._writeSet;
	_exceptSet = other._exceptSet;
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
			FD_SET( s.Fd(), &_readSet );
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

void http::Server::_handleRead( const int fd ) throw( Socket::SocketException )
{
	std::map< int, std::pair< Socket, t_serverData > >::iterator found = _serverSet.find( fd );

	if ( found != _serverSet.end() )
	{
		// this is a new connection
		Socket s = found->second.first.accept();
		FD_SET( s.Fd(), &_readSet );
		_currentData = found->second.second;
	}
	else
	{
		// read from the client
		_currentSock = Socket( fd, true );
		_currentSock.readContent();

		FD_CLR( fd, &_readSet );
		FD_SET( fd, &_writeSet );
	}
}

/*
 *	Fonction qui se charge de la l'envoie des données au client
 *	@Parametres: Le fd sur l'écriture doit se faire
 */
void http::Server::_handleWrite( const int fd )
{
	// write to the client
	_currentSock.serverResponse( _currentData );
	FD_CLR( fd, &_writeSet );
	close( fd );
}

/*
 *	C'est la boucle principale du serveur.Avant chaque lecture/écriture il y a un select()
 *	@Infos: La fonction lève une SocketException si erreur
 *	@Lien: http://manpagesfr.free.fr/man/man2/select.2.html
 */
void http::Server::_watchFds( void ) throw( Socket::SocketException )
{
	fd_set readyReadSet, readyWriteSet;

	_run = true;
	while ( _run )
	{
		// Make a copy because select is destructive !!
		readyReadSet = _readSet;
		readyWriteSet = _writeSet;
		if ( select( FD_SETSIZE, &readyReadSet, &readyWriteSet, NULL, &_timeout ) < 0 )
		{
			if ( _run )
			{
				std::cerr << "Error: select " << strerror( errno ) << std::endl;
				stop();
			}
			exit( EXIT_FAILURE );
		}
		for ( int i = 0; i < FD_SETSIZE; i++ )
		{
			if ( FD_ISSET( i, &readyReadSet ) )
				_handleRead( i );
			if ( FD_ISSET( i, &readyWriteSet ) )
				_handleWrite( i );
		}
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
		for ( int i = 0; i < FD_SETSIZE + 1; i++ )
		{
			if ( FD_ISSET( i, &_readSet ) || FD_ISSET( i, &_writeSet ) )
			{
				Socket( i, true ).close();
			}
		}
	}
}
