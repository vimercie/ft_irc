/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:29:33 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/25 05:09:09 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::invite(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();

	if (msg.getParameters().size() < 2)
		return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS(msg.getCommand()));

	Client*		client = getClientByNickname(msg.getParameters()[0]);
	Channel*	channel = getChannelByName(msg.getParameters()[1]);

	if (client == NULL)
		return sender->appendToSendBuffer(ERR_NOSUCHNICK(msg.getParameters()[0]));

	if (channel == NULL)
		return sender->appendToSendBuffer(ERR_NOSUCHCHANNEL(msg.getParameters()[1]));

	if (!channel->isOperator(sender))
		return sender->appendToSendBuffer(ERR_CHANOPRIVSNEEDED(channel->getName()));

	// if client already invited
	if (channel->isInvited(client))
		return 0;

	channel->sendInvite(client);
	client->appendToSendBuffer(RPL_INVITING(sender->getNickname(), client->getNickname(), channel->getName()));

	return 0;
}
