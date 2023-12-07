/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:39:17 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/06 16:17:26 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

class IRCmsg;
class Client;
class Channel;

class Command
{
	typedef int (Command::*cmd)(const IRCmsg&, Channel*, Client*);

	private:
		std::map<std::string, cmd>	cmds;
	public:
		Command(void);
		~Command();

		int	exec(const IRCmsg& msg, Channel* channel, Client* client);

	// users
		int	cmd_nick(const IRCmsg& msg, Channel* channel, Client* client);
		int	cmd_user(const IRCmsg& msg, Channel* channel, Client* client);
		// int	cmd_pass(const IRCmsg& msg, Channel* channel);
	// channels

	// server
};
