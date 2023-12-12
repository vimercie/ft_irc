/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/12 15:16:15 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Utils.hpp"

void Server::exec(const IRCmsg& msg)
{
    typedef void (Server::*cmd)(const IRCmsg& msg);

    std::map<std::string, cmd> cmds;

    cmds["NICK"] = &Server::nick;
    cmds["USER"] = &Server::user;

	cmds["JOIN"] = &Server::join;

    std::map<std::string, cmd>::iterator it = cmds.find(msg.getCommand());

    if (it != cmds.end())
		(this->*(it->second))(msg);
	else
		std::cout << "Command not found" << std::endl;
}

// user related commands
void	Server::nick(const IRCmsg& msg)
{
	msg.getClient()->setNickname(msg.getParameters()[0]);
	welcome(msg.getClient());
}

void	Server::user(const IRCmsg& msg)
{
	msg.getClient()->setUsername(msg.getParameters()[0]);
	msg.getClient()->setHostname(msg.getParameters()[1]);
	msg.getClient()->setRealname(msg.getTrailing());
}

void	Server::welcome(Client* client)
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
// void	Command::join(const IRCmsg& msg, Server *server)
// {
	
// }

