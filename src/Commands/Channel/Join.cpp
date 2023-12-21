/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:22:54 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/21 12:10:30 by vimercie         ###   ########lyon.fr   */
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
	{
		sender->appendToSendBuffer(ERR_CHANNELISFULL(channel->getName()));
		return 0;
	}
	//if channel is invite only
	if (channel->getMode('i') && !channel->isInvited(sender))
	{
		sender->appendToSendBuffer(ERR_INVITEONLYCHAN(channel->getName()));
		return 0;
	}
	//if channel is password protected
	if (channel->getMode('k'))
	{
		if (msg.getParameters().size() < 2)
		{
			sender->appendToSendBuffer(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), "475", msg.getParameters(), "Password required").toString());
			return 0;
		}

		if (msg.getParameters()[1] != channel->getKey())
		{
			sender->appendToSendBuffer(ERR_BADCHANNELKEY(channel->getName()));
			return 0;
		}
	}

	channel->addClient(sender);

	if (channel->getTopic().empty())
		sender->appendToSendBuffer(RPL_NOTOPIC(channel->getName()));
	else
		sender->appendToSendBuffer(RPL_TOPIC(sender->getNickname() , channel->getName(), channel->getTopic()));
	return 0;
}
