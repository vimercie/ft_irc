/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:27:23 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/09 15:40:50 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int port = atoi(argv[1]);
	std::string password = argv[2];

	Server* myServer = new Server(port, password); // Créer un serveur avec le port et le mot de passe
	myServer->serverLoop();

	delete myServer;
	return 0;
}
