/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/18 14:55:58 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Utils.hpp"
#include "../inc/RPL.hpp"

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
	cmds["TOPIC"] 	= &Server::topic;
	cmds["MODE"] 	= &Server::mode;
	cmds["PART"] 	= &Server::part;

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

	client->appendToSendBuffer(RPL_WELCOME(client->getNickname()));

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

// channel related commands

int	Server::join(const IRCmsg& msg)
{
	Channel*					channel;

	channel = getChannelByName(msg.getParameters()[0]);

	if (channel == NULL)
	{
		channel = new Channel(msg.getParameters()[0]);
		channels.push_back(channel);
		channel->addOperator(msg.getClient());
	}

	channel->addClient(msg.getClient());
	return 0;
}

int Server::privmsg(const IRCmsg& msg)
{
    Client *sender = msg.getClient();
	IRCmsg response;

    if (!sender)
		return 0;

    if (msg.getParameters()[0][0] == '#')	// Si le premier paramètre commence par un #, c'est un canal
	{
        Channel* channel = getChannelByName(msg.getParameters()[0]);

        if (!channel)
			return 0;
		response = msg;
		response.setPrefix(user_id(sender->getNickname(), sender->getUsername()));
        return privmsgToChannel(response, channel);
    }
	else	// Sinon, c'est un client
	{
        Client* recipient = getClientByNickname(msg.getParameters()[0]);

        if (!recipient || recipient == sender)
			return 0;

        return privmsgToClient(msg, recipient);
    }
}

int	Server::topic(const IRCmsg& msg)
{
	Channel*	channel = getChannelByName(msg.getParameters()[0]);
	IRCmsg		response;

	if (channel == NULL)
		return 0;


	if (!msg.getTrailing().empty())
		channel->setTopic(msg.getTrailing());

	response = IRCmsg(msg.getClient(), user_id(msg.getClient()->getNickname(), msg.getClient()->getUsername()), "TOPIC", msg.getParameters(), channel->getTopic());

	channel->sendToChannel(response.toString());

	return 0;
}

int	Server::mode(const IRCmsg& msg)
{
	Channel*	channel = getChannelByName(msg.getParameters()[0]);

	if (channel == NULL)
		return 0;

	channel->setMode(msg.getTrailing()[0], true);

	return 0;
}

int	Server::part(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();
	Channel*	channel = getChannelByName(msg.getParameters()[0]);

	if (channel == NULL)
		return 0;

	channel->removeClient(msg.getClient());
	sender->removeChannel(channel);

	channel->sendToChannel(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), "PART", msg.getParameters(), msg.getTrailing()).toString());

	return 0;
}

// server related commands
int	Server::ping(const IRCmsg& msg)
{
	Client*	client = msg.getClient();

	client->appendToSendBuffer(IRCmsg(client, "localhost", "PONG", msg.getParameters(), msg.getTrailing()).toString());

	return 0;
}


// utils
int Server::privmsgToChannel(const IRCmsg& msg, Channel* channel)
{
	std::vector<Client*>	clients = channel->getClients();
	Client* 				sender = msg.getClient();

   	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		Client* client = *it;

		if (!client || client == sender)
			continue;
		client->appendToSendBuffer(msg.toString());
	}

    return 0;
}

int Server::privmsgToClient(const IRCmsg& msg, Client* recipient)
{
	const Client* sender = msg.getClient();

    IRCmsg response;
    response.setClient(recipient);
    response.setPrefix(user_id(sender->getNickname(), sender->getUsername()));
    response.setCommand("PRIVMSG");
    response.setParameters(std::vector<std::string>(1, recipient->getNickname()));
    response.setTrailing(msg.getTrailing());

    recipient->appendToSendBuffer(response.toString());

    return 0;
}
