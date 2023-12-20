/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:24:06 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/20 18:01:29 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int Server::privmsg(const IRCmsg& msg)
{
	Client				*sender = msg.getClient();
	const std::string	recipient = msg.getParameters()[0];

	if (!sender)
		return 0;

	if (recipient[0] != '#')
	{
		Client* client = getClientByNickname(recipient);

		if (client && client != sender)
			return privmsgToClient(msg, client);
	}

	Channel* channel = getChannelByName(recipient);

	if (!channel)
	{
		sender->appendToSendBuffer(ERR_NOSUCHNICK(recipient));
		return 0;
	}

	IRCmsg response(msg);
	response.setPrefix(user_id(sender->getNickname(), sender->getUsername()));

	return privmsgToChannel(response, channel);
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

int Server::privmsgToChannel(const IRCmsg& msg, Channel* channel)
{
	std::vector<Client*>	clients = channel->getClients();
	Client* 				sender = msg.getClient();

	if (std::find(clients.begin(), clients.end(), sender) == clients.end())
	{
		std::cout << "Client " << sender->getNickname() << " not in channel " << channel->getName() << std::endl;
		sender->appendToSendBuffer(ERR_NOTONCHANNEL(channel->getName()));
		return 0;
	}

   	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		Client* client = *it;

		if (!client || client == sender)
			continue;

		client->appendToSendBuffer(msg.toString());
	}

	return 0;
}
