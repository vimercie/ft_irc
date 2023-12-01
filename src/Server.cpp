/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:16:36 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/01 17:44:03 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/IRCmsg.hpp"

Server::Server(int port, const std::string& password) : port(port), password(password)
{
	// Création du socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		std::cerr << "Erreur de création du socket" << std::endl;
		exit(1);
	}

	// Configuration de l'adresse du serveur
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(this->port);

	// Liaison du socket à l'adresse du serveur
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
	{
		std::cerr << "Erreur de liaison du socket" << std::endl;
		exit(1);
	}

	// Mise en écoute du serveur
	if (listen(sockfd, 5) != 0)
	{
		std::cerr << "Erreur lors de la mise en écoute" << std::endl;
		exit(1);
	}

	// Initialisation du poll
	for (int i = 0; i < MAX_CLIENTS; i++) {
		fds[i].fd = -1;
	}
	fds[0].fd = sockfd;
    fds[0].events = POLLIN;
	nfds = 1;
	std::cout << "Serveur lancé sur le port " << port << std::endl;
}

void Server::acceptConnections() {
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int connfd = accept(sockfd, (struct sockaddr *)&cli, &len);

    if (connfd >= 0) {	
		std::cout << "Nouvelle connexion" << std::endl;
        for (int i = 1; i < nfds; i++) {
            if (fds[i].fd == -1) {
                fds[i].fd = connfd;
                fds[i].events = POLLIN;
				nfds++;
                break;
            }
        }
    }
}

void Server::communicate()
{
	std::cout << "Communicate" << std::endl;
    for (int i = 1; i < nfds; i++) {
        if (fds[i].revents & POLLIN) {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0);

            if (bytes_read > 0) {
                std::string message(buffer, bytes_read);
                std::cout << "Message reçu : " << message << std::endl;
            } else if (bytes_read == 0) {
                std::cout << "Client déconnecté." << std::endl;
                close(fds[i].fd);
                fds[i].fd = -1;  // Marquer comme libre
            } else {
                std::cerr << "Erreur de lecture du socket client." << std::endl;
            }
        }
    }
}

void	Server::serverLoop()
{
	while (true) {
		int ret = poll(fds, nfds, TIMEOUT);

		if (ret < 0) {
			std::cerr << "Erreur de poll" << std::endl;
			break;
		}

		if (fds[0].revents & POLLIN) {
			acceptConnections();
		}
		communicate();
	}
}

Server::~Server()
{
	close(sockfd);
}
