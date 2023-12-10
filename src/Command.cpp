/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/10 17:03:48 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Utils.hpp"

void Command::exec(const IRCmsg& msg)
{
    typedef void (*cmd)(const IRCmsg&);

    static std::map<std::string, cmd> cmds;

    cmds["NICK"] = &Command::nick;
    cmds["USER"] = &Command::user;

    std::map<std::string, cmd>::iterator cmdIt = cmds.find(msg.getCommand());

    if (cmdIt != cmds.end())
        cmdIt->second(msg);
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


// server related commands

