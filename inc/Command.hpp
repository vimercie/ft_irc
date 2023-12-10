/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:39:17 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/10 02:55:00 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# define USER 1
# define CHANNEL 2
# define SERVER 3

# include <iostream>
# include <string>
# include <vector>
# include <map>

class IRCmsg;
class Client;
class Channel;

class Command
{
	typedef void (Command::*userCmd)(const IRCmsg&, Client*);
	// typedef void (Command::*channelCmd)(const IRCmsg&, Channel*);
	// typedef void (Command::*serverCmd)(const IRCmsg&, Server*);

	private:
		std::map<std::string, userCmd>	userCmds;
	public:
		Command(const IRCmsg& msg, Client* client);
		// Command(const IRCmsg& msg, Channel* channel);
		// Command(const IRCmsg& msg, Server* server);
		~Command();

	// methods
		void	exec(const IRCmsg& msg, Client* client);
		// int	exec(const IRCmsg& msg, Channel* channel);
		// int	exec(const IRCmsg& msg, Server* server);

	// users
		void	nick(const IRCmsg& msg, Client* client);
		void	user(const IRCmsg& msg, Client* client);

	// channels

	// server

		void	welcome(Client* client);	
};

#endif
