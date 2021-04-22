/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dess <dboyer@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 14:54:53 by dess              #+#    #+#             */
/*   Updated: 2021/04/22 19:56:02 by dess             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "socket.hpp"
#include <cstdlib>
#include <sys/select.h>
#include <unistd.h>

/******************************************************************************
 *			Constructeurs/Destructeurs
 *****************************************************************************/

/*
 *	Initialise un serveur qui écoutera sur un port donnée
 *	@Parametres: Le port qui sera écouté et l'interval (en seconde) entre chaque écoute
 *	@Lien: http://manpagesfr.free.fr/man/man2/select.2.html
 */
http::Server::Server(uint32_t port, uint32_t timeout) : _port(port)
{
	_timeout.tv_sec = timeout;
	_timeout.tv_usec = 0;
	FD_ZERO(&_readSet);
	FD_ZERO(&_writeSet);
	FD_ZERO(&_exceptSet);
}

http::Server::~Server(void)
{
}

/****************************************************************************************
 *			Fonctions membres
 ***************************************************************************************/

/*
 *	Fonction qui va mettre le serveur en écoute sur le port spécifié dans le constructeur
 *	@Infos: la fonction arrête le programme si une SocketException est levé
 */
void http::Server::listen(void)
{
	try
	{
		_serverSocket.listen(_port);
		FD_SET(_serverSocket.Fd(), &_readSet);
	}
	catch (Socket::SocketException &e)
	{
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	_watchFds();
}

/*
 *	Fonction qui se charge de la lecture des données reçues par le serveur via la socket client
 *	@Parametres: Le fd sur lequel la lecture doit se faire
 *	@Infos La fonction lève une SocketException si erreur
 */
void http::Server::_handleRead(const int fd) throw(Socket::SocketException)
{
	if (_serverSocket == fd)
	{
		Socket clientSocket = _serverSocket.accept();
		FD_SET(clientSocket.Fd(), &_readSet);
	}
	else
	{
		std::string result = Socket(fd, true).readContent(); // Les données reçue sont dans result
		/*
		 * 	Les parsing des headers doit se faire ici
		 */
		std::cout << result;
		FD_CLR(fd, &_readSet);
		FD_SET(fd, &_writeSet);
	}
}

/*
 *	Fonction qui se charge de la l'envoie des données au client
 *	@Parametres: Le fd sur l'écriture doit se faire
 *	@Info: La fonction est vide pour le moment. Mais c'est ici qu'il écrire sur le bon fd
 */
void http::Server::_handleWrite(const int fd)
{
	std::cout << "write fd: " << fd << std::endl;
	FD_CLR(fd, &_writeSet);
}

/*
 *	C'est la boucle principale du serveur.Avant chaque lecture/écriture il y a un select()
 *	@Infos: La fonction lève une SocketException si erreur
 *	@Lien: http://manpagesfr.free.fr/man/man2/select.2.html
 */
void http::Server::_watchFds(void) throw(Socket::SocketException)
{
	fd_set readyReadSet, readyWriteSet;

	while (1)
	{
		// Make a copy because select is destructive !!
		readyReadSet = _readSet;
		readyWriteSet = _writeSet;
		if (select(FD_SETSIZE + 1, &readyReadSet, &readyWriteSet, NULL, &_timeout) < 0)
		{
			std::cerr << "Error: select " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &readyReadSet))
				_handleRead(i);
			if (FD_ISSET(i, &readyWriteSet))
				_handleWrite(i);
		}
	}
}
