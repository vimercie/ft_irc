/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:35:42 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/29 19:37:40 by mmajani          ###   ########lyon.fr   */
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

#define TIMEOUT 500
#define MAX_CLIENTS 10

class IRCMessage;

class Server
{
private:
//server
	int 				sockfd;
	struct sockaddr_in	servaddr;
	int 				port;
	std::string 		password;
//poll
	struct pollfd		fds[MAX_CLIENTS + 1];
	int 				nfds;

public:
	Server(int port, const std::string& password);
	~Server();

	void	acceptConnections();
	void	communicate();
	void	serverLoop();
};

#endif