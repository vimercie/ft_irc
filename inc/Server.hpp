/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:35:42 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/03 19:39:15 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>

#define TIMEOUT 500
#define MAX_CLIENTS 10

class Channel;

class Server
{
private:
// server
	std::string				name;
// socket
	int 					sockfd;
	struct sockaddr_in		servaddr;
	int 					port;
	std::string 			password;
// poll
	struct pollfd			fds[MAX_CLIENTS + 1];
	int 					nfds;
// channels
	std::vector<Channel>	channels;

public:
	Server(int port, const std::string& password);
	~Server();

// getters
	std::string	getName() const;

	void	initialize();
	void	acceptConnections();
	void	communicate();
	void	serverLoop();
};

#endif