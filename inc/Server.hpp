/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:35:42 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/09 17:05:26 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define MAX_CLIENTS 10

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <unistd.h>
# include <cstring>
# include <cstdlib>
# include <poll.h>
# include <fcntl.h>
# include <errno.h>
# include <vector>
# include <signal.h>

class Channel;
class Client;
class IRCmsg;

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
		nfds_t 					nfds;
	// channels
		std::vector<Channel*>	channels;
		std::vector<Client*>	clients;

	// methods
		void	initialize();
		void	acceptConnections();
		void	closeServer();

	public:
		Server(int port, const std::string& password);
		~Server();

	// getters
		std::string	getName() const;

	// methods
		void					serverLoop();
		Client					initClient(int i, struct pollfd* fds);
		void					addClient(pollfd *socket);
		// void					removeClient(Client* client);
		Client*					getClientByFd(int fd);

		void					communicate();
		std::vector<IRCmsg*>	readMsg(int	fd);
		void					sendMsg(int fd, const std::string& msg);

		void					welcome(Client* client);
};

#endif