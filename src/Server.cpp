/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 13:07:18 by vimercie          #+#    #+#             */
/*   Updated: 2023/09/25 14:48:18 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"

Server::Server(const std::string& port, const std::string& password)
{
	_portno = std::atoi(port.c_str());
	_password = password;

	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = PF_INET;
	_addr.sin_port = htons(_portno);
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addrLen = sizeof(_addr);

	_channels.push_back(Channel("general"));

	if (_portno < 1024 || _portno > 65535)
		throw std::runtime_error("Invalid port number");
	if (_password.size() > 50)
		throw std::runtime_error("Password too long");
}

Server::~Server() {}

void Server::init()
{
	int	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
		throw std::runtime_error("socket creation failed");
	if (bind(sockfd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw std::runtime_error("socket binding failed");
	if (listen(sockfd, 10) == -1)
		throw std::runtime_error("Unable to listen to socket");

	std::cout << "Server is listening on port " << _portno << std::endl;

	while (1)
	{
		sockaddr_in	clientAddr;
		socklen_t	clientAddrLen = sizeof(clientAddr);
		int			newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);

		if (newsockfd == -1)
			throw std::runtime_error("Unable to accept connection");

		std::cout << "New connection" << std::endl;

		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);

		if (recv(newsockfd, buffer, BUFFER_SIZE, 0) == -1)
			throw std::runtime_error("Error receiving message");

		std::cout << "Client: " << buffer << std::endl;

		bzero(buffer, BUFFER_SIZE);

		std::cout << "Enter message: ";

		std::cin >> buffer;
		if (send(newsockfd, buffer, strlen(buffer), 0) == -1)
			throw std::runtime_error("Error sending message");

		close(newsockfd);
	}
}
