/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:16:36 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/15 04:08:25 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Utils.hpp"

int	status = 1;
void statusHandler(int sig);

Server::Server(int port, const std::string& password) : port(port), password(password)
{
	std::cout << "Server created" << std::endl;

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, statusHandler);

	createSocket();
	configureServerAddress();
	bindSocket();
	startListening();
	initializePoll();

	name = "localhost";

	std::cout << "Serveur lancé sur le port " << port << std::endl;
}

Server::~Server()
{
	// Fermeture du socket serveur
	close(sockfd);

	// Suppression des channels
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
		delete *it;
	channels.clear();

	// Suppression des clients
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		delete *it;
	clients.clear();

	std::cout << "Server destroyed" << std::endl;
}

// getters
std::vector<Channel*>	Server::getChannels() const {return channels;}


void	Server::createSocket()
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		std::cerr << "Erreur de création du socket" << std::endl;
		exit(1);
	}

	// Configurer SO_REUSEADDR
	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	// Configurer TCP_NODELAY
	int nodelay = 1;
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
}

void	Server::configureServerAddress()
{
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
}

void	Server::bindSocket()
{
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
	{
		std::cerr << "Erreur de liaison du socket" << std::endl;
		exit(1);
	}
}

void	Server::startListening()
{
	if (listen(sockfd, 5) != 0)
	{
		std::cerr << "Erreur lors de la mise en écoute" << std::endl;
		exit(1);
	}
}

void	Server::initializePoll()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		fds[i].fd = -1;
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}

	fds[0].fd = sockfd;

	nfds = 1;
}

void	Server::serverLoop()
{
	int	poll_ret;

	while (status)
	{
		poll_ret = poll(fds, nfds, 0);

		if (poll_ret < 0 && status == 1)
		{
			std::cerr << "Erreur de poll" << std::endl;
			break;
		}

		if (fds[0].revents & POLLIN)
			acceptConnections();

		communicate();
	}
}

void	Server::acceptConnections()
{
	struct sockaddr_in	cli;
	socklen_t			len = sizeof(cli);
	int					connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	int					sockIndex = 1;

	if (connfd < 0)
	{
		std::cerr << "Erreur de connexion" << std::endl;
		return;
	}

	while (fds[sockIndex].fd != -1 && sockIndex < MAX_CLIENTS)
		sockIndex++;

	if (sockIndex == MAX_CLIENTS)
	{
		std::cout << "Trop de clients connectés" << std::endl;
		return;
	}

	fds[sockIndex].fd = connfd;
	fds[sockIndex].events = POLLIN;
	fds[sockIndex].revents = 0;

	clients.push_back(new Client(&fds[sockIndex]));

	nfds++;

	std::cout << "Nouvelle connexion acceptée" << std::endl;
}

void	Server::closeConnection(int fd)
{
	for (nfds_t i = 1; i <= nfds; i++)
	{
		if (fds[i].fd == fd)
		{
			close(fd);
			fds[i].fd = -1;
			break;
		}
	}
	nfds--;
}

void	Server::removeClient(Client* client)
{
	// Fermeture de la connexion
	closeConnection(client->getSocket().fd);

    // Suppression du client des canaux
	std::vector<Channel*> channelsCopy = client->getChannels();
    for (std::vector<Channel*>::iterator it = channelsCopy.begin(); it != channelsCopy.end(); it++)
		(*it)->removeClient(client);

	// Suppression du client de la liste des clients
	std::vector<Client*>::iterator it = std::find(clients.begin(), clients.end(), client);
	if (it != clients.end())
	{
		delete *it;
		clients.erase(it);
	}
}

void	Server::communicate()
{
	std::vector<IRCmsg*>	messages;

	for (nfds_t i = 1; i < nfds; i++)
	{
		// Si le socket est libre
		if (fds[i].fd == -1)
			continue;

		// Si le socket a une erreur
		if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
		{
			close(fds[i].fd);
			fds[i].fd = -1;
			continue;
		}

		// Si le socket est prêt à être lu
		if (fds[i].revents & POLLIN)
		{
			messages = readMsg(fds[i].fd);

			// Lecture des messages entrants
			for (std::vector<IRCmsg*>::iterator it = messages.begin(), end = messages.end(); it != end; it++)
			{
				// Affichage du message reçu
				std::cout << "<" + (*it)->getClient()->getNickname() + ">" + " : " + (*it)->toString();

				// Exécution des commandes
				if (exec(*(*it)) != 0)
					break;
			}
			for (std::vector<IRCmsg*>::iterator it = messages.begin(); it != messages.end(); it++)
				delete *it;
			messages.clear();
		}
	}
}

std::vector<IRCmsg*>	Server::readMsg(int fd)
{
	char					buffer[1024];
	ssize_t					bytes_read = 0;
	std::vector<IRCmsg*>	res;

	memset(buffer, 0, sizeof(buffer));

	bytes_read = recv(fd, buffer, sizeof(buffer), MSG_DONTWAIT);

	// Si on a reçu des données
	if (bytes_read > 0)
	{
		std::vector<std::string>	msgs = splitString(buffer, "\r\n");

		for (std::vector<std::string>::iterator it = msgs.begin(); it != msgs.end(); it++)
		{
			if (it->empty())
				continue;

			res.push_back(new IRCmsg(getClientByFd(fd), *it));
		}
	}
	// Si le client s'est déconnecté
	else if (bytes_read == 0)
	{
		std::cout << "Client déconnecté." << std::endl;
		close(fd);
		fds[fd].fd = -1;  // Marquer comme libre
	}
	// Si on a une erreur
	else if (errno != EWOULDBLOCK)		// CONDITION INTERDITE (À supprimer) (utiliser fcntl(fd, F_SETFL, O_NONBLOCK) à la place ?)
		std::cerr << "Erreur de lecture du socket client." << std::endl;

	return res;
}

void	Server::sendMsg(int fd, const std::string& msg)
{
	if (send(fd, msg.c_str(), msg.length(), 0) < 0)
		std::cerr << "Erreur d'envoi du message." << std::endl;
}

// utils
Client*	Server::getClientByFd(int fd)
{
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getSocket().fd == fd)
			return *it;
	}

	return NULL;
}

Client*	Server::getClientByNickname(const std::string& nickname)
{
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			return *it;
	}

	return NULL;
}

Channel*	Server::getChannelByName(const std::string& name)
{
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
		if ((*it)->getName() == name)
			return *it;
	return NULL;
}
