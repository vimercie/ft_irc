/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:16:36 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/21 12:54:16 by vimercie         ###   ########lyon.fr   */
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

	cmdsInit();

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
const std::vector<Client*>&		Server::getClients() const {return clients;}
const std::vector<Channel*>&	Server::getChannels() const {return channels;}


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
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		std::cerr << "Erreur de bind" << std::endl;
		delete this;
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

void	Server::cmdsInit()
{
	cmds["NICK"] = &Server::nick;
	cmds["USER"] = &Server::user;
	cmds["PASS"] = &Server::pass;
	cmds["QUIT"] = &Server::quit;

	cmds["JOIN"] 	= &Server::join;
	cmds["PRIVMSG"]	= &Server::privmsg;
	cmds["TOPIC"] 	= &Server::topic;
	cmds["MODE"] 	= &Server::mode;
	cmds["PART"] 	= &Server::part;
	cmds["KICK"] 	= &Server::kick;
	cmds["INVITE"] 	= &Server::invite;

	cmds["PING"] = &Server::ping;
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
	fds[sockIndex].events = POLLIN | POLLOUT;
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

void Server::removeClient(Client* client)
{
	// Fermeture de la connexion
	closeConnection(client->getSocket().fd);

	// Suppression du client des canaux
	std::vector<Channel*> channelsCopy = client->getChannels();
	for (std::vector<Channel*>::iterator it = channelsCopy.begin(); it != channelsCopy.end(); it++)
		(*it)->removeClient(client);

	// Suppression du client de la liste des clients
	delete client;
	clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
}

void Server::removeClients()
{
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); )
	{
		if ((*it)->isToDisconnect())
		{
			removeClient(*it);
			continue;
		}
		else
			++it;
	}
}

void Server::removeChannel(Channel* channel)
{
	// Suppression du canal de la liste des canaux
	delete channel;
	channels.erase(std::remove(channels.begin(), channels.end(), channel), channels.end());
}

void Server::removeEmptyChannels()
{
	if (channels.empty())
		return;

    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); )
	{
        if ((*it)->getClients().empty())
		{
            delete *it;  // Supprime le canal si nécessaire
            it = channels.erase(it);  // Supprime le pointeur du vecteur et avance l'itérateur
        }
		else
            ++it;
    }
}

void Server::serverLoop()
{
    int poll_ret;

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

        for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			// Vérifie si le client n'est pas nul
            if (!(*it))
				continue;

			// Lecture des messages entrants
            if ((*it)->getSocket().revents & POLLIN)
                (*it)->readFromSocket();

			// Traitement des commandes et génération des réponses
            processCommands(*it);

			// Envoi des réponses
			if ((*it)->getSocket().revents & POLLOUT)
            	(*it)->sendToSocket();
        }

        // Supprime les clients et les channels marqués pour suppression
        removeClients();
		removeEmptyChannels();
    }
}

void	Server::processCommands(Client *client)
{
    std::vector<std::string>	recvData = client->getRecvBuffer();

	if (recvData.empty())
		return;

	for (std::vector<std::string>::iterator it = recvData.begin(); it != recvData.end(); ++it)
	{
		std::cout << "<" + client->getNickname() + ">" + " : " + *it;

		IRCmsg msg(client, *it);

		if (exec(msg) != 0)
			break;
	}

    client->clearRecvBuffer(); // Effacer le buffer de réception après le traitement
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
	std::string	channel_name = name;

	if (name[0] != '#')
		channel_name = "#" + name;

	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if ((*it)->getName() == channel_name)
			return *it;
	}

	return NULL;
}
