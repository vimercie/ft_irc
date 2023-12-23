/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:22:54 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 16:39:51 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::join(const IRCmsg& msg)
{
	Channel*	channel= getChannelByName(msg.getParameters()[0]);
	Client*		sender = msg.getClient();

	if (channel == NULL)
	{
		channel = new Channel(msg.getParameters()[0]);
		channels.push_back(channel);
		channel->addOperator(sender);
	}

	//if channel limit reached
	if (channel->getLimit() != 0 && channel->getClients().size() + 1 > channel->getLimit())
		return sender->appendToSendBuffer(ERR_CHANNELISFULL(channel->getName()));

	//if channel is invite only
	if (channel->getMode('i') && !channel->isInvited(sender))
		return sender->appendToSendBuffer(ERR_INVITEONLYCHAN(channel->getName()));

	//if channel is password protected
	if (channel->getMode('k'))
	{
		if (msg.getParameters().size() < 2)
			return sender->appendToSendBuffer(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), "475", msg.getParameters(), "Password required").toString());

		if (msg.getParameters()[1] != channel->getKey())
			return sender->appendToSendBuffer(ERR_BADCHANNELKEY(channel->getName()));
	}

	channel->addClient(sender);

	if (channel->getTopic().empty())
		return sender->appendToSendBuffer(RPL_NOTOPIC(channel->getName()));
	else
		return sender->appendToSendBuffer(RPL_TOPIC(sender->getNickname() , channel->getName(), channel->getTopic()));
}
