/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/06 16:17:10 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"

Command::Command()
{
	cmds["NICK"] = cmd_nick;
	cmds["USER"] = cmd_user;
	// cmds["PASS"] = cmd_pass;
}

int	Command::exec(const IRCmsg& msg, Channel* channel, Client* client)
{
	std::map<std::string, cmd>::iterator	it = std::find(cmds.begin(), cmds.end(), msg.getCommand());

	if (it == cmds.end())
		return (0);

	int ret = (this->*(it->second))(msg, channel, client);
}

int	Command::cmd_nick(const IRCmsg& msg, Channel* channel, Client* client)
{
		if (msg.getParameters().size() != 1
		|| msg.getParameters()[0].empty()
		|| msg.getParameters()[0].length() > 9)
		return ;

	std::vector<Client>::iterator	it = std::find(channel->getClients().begin(), channel->getClients().end(), msg.getParameters()[0]);

	if (it == channel->getClients().end())
		channel->addClient(Client(msg.getParameters()[0]));
	else
		it->setNickname(msg.getParameters()[0]);
}

int	Command::cmd_user(const IRCmsg& msg, Channel* channel, Client* client)
{
	std::string username = msg.getParameters()[0];
	std::string realname = msg.getParameters()[3];

	client->setUsername(username);
	client->setRealname(realname);
}

// int	Command::cmd_pass(const IRCmsg& msg, Channel* channel)
// {
// 	std::string password = msg.getParameters()[0];
// 	client->setPassword(password);
// }

/*

void	Channel::nick(const IRCmsg& msg)
{
	if (msg.getParameters().size() != 1
		|| msg.getParameters()[0].empty()
		|| msg.getParameters()[0].length() > NICKNAME_MAX_LENGTH)
		return ;

	std::vector<Client>::iterator	it = std::find(clients.begin(), clients.end(), msg.getParameters()[0]);

	if (it == clients.end())
		addClient(msg.getParameters()[0]);
	else
		it->setNickname(msg.getParameters()[0]);
}

void	Channel::user(const IRCmsg& msg)
{
	std::vector<Client>::iterator	it = std::find(clients.begin(), clients.end(), msg.getParameters()[0]);

	if (it == clients.end())
		return ;
	else
		it->setUsername(msg.getParameters()[0]);

	welcome(msg);
}

*/
