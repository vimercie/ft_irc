/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:24:06 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 16:29:52 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int privmsgToClient(const IRCmsg& msg, Client* recipient)
{
	Client* sender = msg.getClient();

	if (!sender || !recipient)
		return 0;

	IRCmsg response;
	response.setClient(sender);
	response.setPrefix(user_id(sender->getNickname(), sender->getUsername()));
	response.setCommand(msg.getCommand());
	response.setParameters(std::vector<std::string>(1, recipient->getNickname()));
	response.setTrailing(msg.getTrailing());

	recipient->appendToSendBuffer(response.toString());

	return 0;
}

int privmsgToChannel(const IRCmsg& msg, Channel* channel)
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

int Server::privmsg(const IRCmsg& msg)
{
	Client				*sender = msg.getClient();

	if (!sender)
		return 0;

	if (msg.getParameters().empty())
		return sender->appendToSendBuffer(ERR_NORECIPIENT("PRIVMSG"));

	if (msg.getParameters().size() > 1)
		return sender->appendToSendBuffer(ERR_TOOMANYTARGETS("PRIVMSG"));

	if (msg.getTrailing().empty())
		return sender->appendToSendBuffer(ERR_NOTEXTTOSEND());

	const std::string	recipient = msg.getParameters()[0];

	if (recipient[0] != '#')
	{
		Client* client = getClientByNickname(recipient);

		if (client && client != sender)
			return privmsgToClient(msg, client);
	}

	Channel* channel = getChannelByName(recipient);

	if (!channel)
		return sender->appendToSendBuffer(ERR_NOSUCHNICK(recipient));

	IRCmsg response(msg);
	response.setPrefix(user_id(sender->getNickname(), sender->getUsername()));

	return privmsgToChannel(response, channel);
}
