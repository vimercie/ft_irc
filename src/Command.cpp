/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/17 00:09:40 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Utils.hpp"

int Server::exec(const IRCmsg& msg)
{
    typedef int (Server::*cmd)(const IRCmsg& msg);

    std::map<std::string, cmd> cmds;

    cmds["NICK"] = &Server::nick;
    cmds["USER"] = &Server::user;
	cmds["PASS"] = &Server::pass;
	cmds["QUIT"] = &Server::quit;

	cmds["JOIN"] 	= &Server::join;
	cmds["PRIVMSG"]	= &Server::privmsg;

	cmds["PING"] = &Server::ping;

    std::map<std::string, cmd>::iterator it = cmds.find(msg.getCommand());

    if (it != cmds.end())
		return (this->*(it->second))(msg);
	else
		std::cout << "Command not found" << std::endl;

	std::cout << std::endl;

	return 0;
}

// user related commands
int	Server::nick(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	if (!client->isAuthenticated())
	{
		std::cout << "Client " << client->getNickname() << " NOT authenticated" << std::endl;
		client->appendToSendBuffer(err_notregistered());
		client->setToDisconnect(true);
		return 1;
	}

	client->setNickname(msg.getParameters()[0]);

	welcome(client);

	return 0;
}

int	Server::user(const IRCmsg& msg)
{
	msg.getClient()->setUsername(msg.getParameters()[0]);
	msg.getClient()->setHostname(msg.getParameters()[1]);
	msg.getClient()->setRealname(msg.getTrailing());

	return 0;
}

int	Server::pass(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	if (msg.getParameters()[0] != this->password)
	{
		std::cout << "Wrong password" << std::endl;
		client->appendToSendBuffer(err_passwdmismatch());
		client->setToDisconnect(true);
		return 1;
	}

	client->setPass(true);

	return 0;
}

int	Server::quit(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	std::cout << "Client " << client->getNickname() << " quit (" << msg.getTrailing() << ")" << std::endl;
	client->setToDisconnect(true);

	return 0;
}

int	Server::join(const IRCmsg& msg)
{
	Channel*					channel;

	channel = getChannelByName(msg.getParameters()[0]);

	if (channel == NULL)
	{
		channel = new Channel(msg.getParameters()[0]);
		channels.push_back(channel);
	}

	channel->addClient(msg.getClient());
	return 0;
}

int Server::privmsg(const IRCmsg& msg)
{
    Client *sender = msg.getClient();

    if (!sender)
		return 0;

    if (msg.getParameters()[0][0] == '#')	// Si le premier paramètre commence par un #, c'est un canal
	{
        Channel* channel = getChannelByName(msg.getParameters()[0]);

        if (!channel)
			return 0;

        return privmsgToChannel(msg, channel, sender);
    }
	else	// Sinon, c'est un client
	{
        Client* recipient = getClientByNickname(msg.getParameters()[0]);

        if (!recipient || recipient == sender)
			return 0;

        return privmsgToClient(msg, recipient, sender);
    }
}

int Server::privmsgToChannel(const IRCmsg& msg, Channel* channel, Client* sender)
{
    const std::vector<Client*>& clients = channel->getClients();

    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
        Client* recipient = *it;

        if (!recipient || recipient == sender)
			continue;

        IRCmsg response;
        response.setPrefix(sender->getNickname());
        response.setCommand("PRIVMSG");
        response.setParameters(std::vector<std::string>(1, channel->getName()));
        response.setTrailing(msg.getTrailing());
        response.setClient(recipient);

        recipient->appendToSendBuffer(response.toString());
    }

    return 0;
}

int Server::privmsgToClient(const IRCmsg& msg, Client* recipient, Client* sender)
{
    IRCmsg response;
    response.setPrefix(sender->getNickname());
    response.setCommand("PRIVMSG");
    response.setParameters(std::vector<std::string>(1, recipient->getNickname()));
    response.setTrailing(msg.getTrailing());
    response.setClient(recipient);

    recipient->appendToSendBuffer(response.toString());

    return 0;
}

int	Server::welcome(Client* client)
{
	IRCmsg	msg;

	msg.setCommand("001");
	msg.setPrefix("localhost");
	msg.setParameters(std::vector<std::string>(1, client->getNickname()));
	msg.setTrailing("Wesh wesh wesh " + client->getNickname());

	client->appendToSendBuffer(msg.toString());

	return 0;
}

// channel related commands

// server related commands
int	Server::ping(const IRCmsg& msg)
{
	IRCmsg	response;

	response.setPrefix("localhost");
	response.setCommand("PONG");
	response.setParameters(msg.getParameters());
	response.setTrailing(msg.getTrailing());
	response.setClient(msg.getClient());

	sendMsg(msg.getClient()->getSocket().fd, response.toString());

	return 0;
}

