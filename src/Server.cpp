/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 18:16:36 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/15 18:27:12 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(int port, const std::string& password) : port(port), password(password)
{
    // Création du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Erreur de création du socket" << std::endl;
        exit(1);
    }

    // Configuration de l'adresse du serveur
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(this->port);

    // Liaison du socket à l'adresse du serveur
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        std::cerr << "Erreur de liaison du socket" << std::endl;
        exit(1);
    }

    // Mise en écoute du serveur
    if (listen(sockfd, 5) != 0) {
        std::cerr << "Erreur lors de la mise en écoute" << std::endl;
        exit(1);
    }
}

void Server::acceptConnections()
{
    struct sockaddr_in cli;
    int len = sizeof(cli);
    int connfd;

    std::cout << "En attente de connexions..." << std::endl;

    // Accepter une connexion client
    connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t *)&len);
    if (connfd < 0) {
        std::cerr << "Serveur accepte erreur de connexion" << std::endl;
        exit(1);
    } else {
        std::cout << "Client connecté." << std::endl;
    }

    // Communiquer avec le client
    communicate(connfd);

    // Fermeture de la connexion client
    close(connfd);
}

void Server::communicate(int connfd)
{
    const int bufferSize = 256;
    char buffer[bufferSize];

    while (true)
	{
        memset(buffer, 0, bufferSize);

        // Réception d'un message du client
        int bytesReceived = read(connfd, buffer, bufferSize - 1);
        if (bytesReceived <= 0) {
            // Si aucune donnée n'est reçue ou si une erreur se produit, sortez de la boucle
            break;
        }

        // Afficher le message reçu dans le terminal
        std::cout << "Message reçu : " << buffer << std::endl;

        // Envoyer une réponse au client
        const char* response = "Message reçu.";
        write(connfd, response, strlen(response));
    }

    std::cout << "Connexion fermée avec le client." << std::endl;
}

Server::~Server()
{
    close(sockfd);
}
