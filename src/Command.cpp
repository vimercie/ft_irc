/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/10 17:28:14 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Utils.hpp"

void Command::exec(Server* server, const IRCmsg& msg)
{
    typedef void (*userCmd)(const IRCmsg&);
	typedef void (*chanCmd)(const IRCmsg&, std::vector<Channel*>);
	// typedef void (*servCmd)(const IRCmsg&);

    static std::map<std::string, userCmd> userCmds;
	static std::map<std::string, chanCmd> chanCmds;
	// static std::map<std::string, servCmd> servCmds;

    userCmds["NICK"] = &Command::nick;
    userCmds["USER"] = &Command::user;

	// chanCmds["JOIN"] = &Command::join;

    std::map<std::string, userCmd>::iterator userCmdIt = userCmds.find(msg.getCommand());

    if (userCmdIt != userCmds.end())
	{
        userCmdIt->second(msg);
	}

	std::map<std::string, chanCmd>::iterator chanCmdIt = chanCmds.find(msg.getCommand());

	if (chanCmdIt != chanCmds.end())
	{
		chanCmdIt->second(msg, server->getChannels());
	}
}

// user related commands
void	Command::nick(const IRCmsg& msg)
{
	msg.getClient()->setNickname(msg.getParameters()[0]);
	welcome(msg.getClient());
}

void	Command::user(const IRCmsg& msg)
{
	msg.getClient()->setUsername(msg.getParameters()[0]);
	msg.getClient()->setHostname(msg.getParameters()[1]);
	msg.getClient()->setRealname(msg.getTrailing());
}

void	Command::welcome(Client* client)
{
	IRCmsg	msg;

	msg.setCommand("001");
	msg.setPrefix("localhost");
	msg.setParameters(std::vector<std::string>(1, client->getNickname()));
	msg.setTrailing("Wesh wesh wesh " + client->getNickname());

	sendMsg(client->getSocket().fd, msg.toString());
}


// channel related commands
// void	Command::join(const IRCmsg& msg)
// {
	
// }

// server related commands

