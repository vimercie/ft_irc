/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:22:54 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/25 05:13:19 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::join(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();

	if (msg.getParameters().empty())
		return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS("JOIN"));

	Channel*	channel= getChannelByName(msg.getParameters()[0]);

	if (channel == NULL)
	{
		channel = new Channel(msg.getParameters()[0]);
		channels.push_back(channel);
		channel->addOperator(sender);
	}

	//if channel limit reached
	if (channel->getMode('l') && channel->getClients().size() >= channel->getLimit())
		return sender->appendToSendBuffer(ERR_CHANNELISFULL(channel->getName()));

	//if channel is invite only
	if (channel->getMode('i') && !channel->isInvited(sender))
		return sender->appendToSendBuffer(ERR_INVITEONLYCHAN(channel->getName()));

	//if channel is password protected
	if (channel->getMode('k'))
	{
		if (msg.getParameters().size() < 2)
			return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS("JOIN"));

		if (msg.getParameters()[1] != channel->getKey())
			return sender->appendToSendBuffer(ERR_BADCHANNELKEY(channel->getName()));
	}

	channel->addClient(sender);

	channel->sendToChannel(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), msg.getCommand(), msg.getParameters(), msg.getTrailing()).toString());

	if (channel->getTopic().empty())
		return sender->appendToSendBuffer(RPL_NOTOPIC(channel->getName()));
	else
		return sender->appendToSendBuffer(RPL_TOPIC(sender->getNickname() , channel->getName(), channel->getTopic()));
}
