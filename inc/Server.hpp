/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:35:42 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/12 16:56:46 by vimercie         ###   ########lyon.fr   */
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

		std::vector<Channel*>	channels;
		std::vector<Client*>	clients;

	// methods

	// server init
		void	createSocket();
		void	configureServerAddress();
		void	bindSocket();
		void	startListening();
		void	initializePoll();

		void	acceptConnections();
		void	closeConnection(int fd);
		void	removeClient(Client* client);

	// cmds
		int	exec(const IRCmsg& msg);

		int	nick(const IRCmsg& msg);
		int	user(const IRCmsg& msg);
		int	pass(const IRCmsg& msg);
		int	quit(const IRCmsg& msg);

		int	join(const IRCmsg& msg);
		int	privmsg(const IRCmsg& msg);

		void	welcome(Client* client);

	// errors
		int	err_passwdmismatch(const IRCmsg& msg);

	public:
		Server(int port, const std::string& password);
		~Server();

	// getters
		std::vector<Channel*>	getChannels() const;

	// methods
		void					serverLoop();

		void					communicate();
		std::vector<IRCmsg*>	readMsg(int	fd);
		void					sendMsg(int fd, const std::string& msg);

	// channels broadcast
		void					broadcast(const IRCmsg& msg);
		void					broadcast(const IRCmsg& msg, const std::vector<Client*>& clients);
		void					broadcast(const IRCmsg& msg, const std::vector<Channel*>& channels);
		void					broadcast(const IRCmsg& msg, const std::vector<Client*>& clients, const std::vector<Channel*>& channels);


	// utils
		Client*					getClientByFd(int fd);
		Channel*				getChannelByName(const std::string& name);
};

#endif