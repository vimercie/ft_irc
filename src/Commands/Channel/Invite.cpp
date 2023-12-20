/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:29:33 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/20 18:01:58 by vimercie         ###   ########lyon.fr   */
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
	Client*		client = getClientByNickname(msg.getParameters()[0]);
	Channel*	channel = getChannelByName(msg.getParameters()[1]);

	if (client == NULL || channel == NULL)
		return 0;

	if (!channel->isOperator(sender))
		return 0;

	// if client already invited
	if (channel->isInvited(client))
		return 0;

	channel->sendInvite(client);
	client->appendToSendBuffer(RPL_INVITING(client->getNickname(), channel->getName()));
	client->appendToSendBuffer(RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));

	return 0;
}
