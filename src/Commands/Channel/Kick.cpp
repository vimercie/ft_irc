/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:30:12 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/21 11:16:49 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int Server::kick(const IRCmsg& msg)
{
	Channel*	channel = getChannelByName(msg.getParameters()[0]);
	Client*		client = getClientByNickname(msg.getParameters()[1]);
	Client*		sender = msg.getClient();

	if (client == NULL || channel == NULL)
	{
		std::cout << "client or channel not found" << std::endl;
		return 0;
	}

	if (!channel->isOperator(sender))
	{
		std::cout << sender->getNickname() << " is not operator of channel " << channel->getName() << std::endl;
		return 0;
	}

	std::cout << "kicking " << client->getNickname() << " from channel " << channel->getName() << std::endl;

	channel->sendToChannel(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), "KICK", msg.getParameters(), msg.getTrailing()).toString());

	return 0;
}
