/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:39:17 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/08 18:56:25 by vimercie         ###   ########lyon.fr   */
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
	typedef int (Command::*userCmd)(const IRCmsg&, Client*, Channel*);

	private:
		std::map<std::string, userCmd>	userCmds;
	public:
		Command(int type);
		~Command();

	// users
		int	nick(const IRCmsg& msg, Client* client, Channel* channel);
		int	user(const IRCmsg& msg, Client* client, Channel* channel);

	// channels

	// server
	
};

#endif
