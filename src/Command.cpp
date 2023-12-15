/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/15 00:30:17 by vimercie         ###   ########lyon.fr   */
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
	if (!msg.getClient()->isAuthenticated())
	{
		std::cout << "Client " << msg.getClient()->getNickname() << " NOT authenticated" << std::endl;
		sendMsg(msg.getClient()->getSocket().fd, err_passwdmismatch());
		removeClient(msg.getClient());
		return 1;
	}

	msg.getClient()->setNickname(msg.getParameters()[0]);

	welcome(msg.getClient());

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
	if (msg.getParameters()[0] != this->password)
	{
		std::cout << "Wrong password" << std::endl;
		sendMsg(msg.getClient()->getSocket().fd, err_passwdmismatch());
		removeClient(msg.getClient());
		return 1;
	}

	msg.getClient()->setPass(true);

	return 0;
}

int	Server::quit(const IRCmsg& msg)
{
	std::cout << "Client " << msg.getClient()->getNickname() << " quit (" << msg.getTrailing() << ")" << std::endl;
	removeClient(msg.getClient());

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


int	Server::privmsg(const IRCmsg& msg)
{
	IRCmsg						response;
	Channel*					channel;

	channel = getChannelByName(msg.getParameters()[0]);
	if (channel == NULL)
	{
		std::cout << "Channel " << msg.getParameters()[0] << " not found" << std::endl;
		return 1;
	}
	else
	{
		response.setPrefix(msg.getClient()->getNickname());
		response.setCommand(msg.getCommand());
		response.setParameters(msg.getParameters());
		response.setTrailing(msg.getTrailing());
		response.setClient(msg.getClient());
		std::cout << response.toString();
		std::vector<Client*> clients = channel->getClients();
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if ((*it)->getSocket().fd != msg.getClient()->getSocket().fd)
		    	sendMsg((*it)->getSocket().fd, response.toString());
		}
	}
	return 0;
}


int	Server::welcome(Client* client)
{
	IRCmsg	msg;

	msg.setCommand("001");
	msg.setPrefix("localhost");
	msg.setParameters(std::vector<std::string>(1, client->getNickname()));
	msg.setTrailing("Wesh wesh wesh " + client->getNickname());

	sendMsg(client->getSocket().fd, msg.toString());

	return 0;
}

// channel related commands

// server related commands
// void	Command::join(const IRCmsg& msg, Server *server)
// {
	
// }

