/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:35:42 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/19 19:34:05 by mmajani          ###   ########lyon.fr   */
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
# include <netinet/tcp.h>
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
	
	// methods
		void	processCommands(Client* client);

	// cmds
		int	exec(const IRCmsg& msg);

		// user related commands
		int		nick(const IRCmsg& msg);
		int		user(const IRCmsg& msg);
		int		pass(const IRCmsg& msg);
		int		quit(const IRCmsg& msg);
	
		// channel related commands
		int		join(const IRCmsg& msg);
		int		privmsg(const IRCmsg& msg);
		int		topic(const IRCmsg& msg);
		int 	part(const IRCmsg& msg);
		int 	mode(const IRCmsg& msg);
		int		invite(const IRCmsg& msg);
		int		uninvite(const IRCmsg& msg);

		// server related commands
		int	ping(const IRCmsg& msg);

		// utils
		int	privmsgToChannel(const IRCmsg& msg, Channel* channel);
		int	privmsgToClient(const IRCmsg& msg, Client* receiver);

	// errors
		std::string	err_passwdmismatch();
		std::string	err_unknowncommand(const IRCmsg& msg);
		std::string	err_notregistered();

	public:
		Server(int port, const std::string& password);
		~Server();

	// getters
		const std::vector<Client*>&		getClients() const;
		const std::vector<Channel*>&	getChannels() const;

	// methods
		void					serverLoop();

		void					communicate();
		void					sendMsg(int fd, const std::string& msg);

	// utils
		Client*					getClientByFd(int fd);
		Client*					getClientByNickname(const std::string& nickname);
		Channel*				getChannelByName(const std::string& name);
};

#endif