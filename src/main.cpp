/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:27:23 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/29 19:42:16 by mmajani          ###   ########lyon.fr   */
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

	Server myServer(port, password); // Créer un serveur avec le port et le mot de passe
	myServer.serverLoop();
	return 0;
}
