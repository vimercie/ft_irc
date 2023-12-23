/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:30:12 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 16:42:18 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int Server::kick(const IRCmsg& msg)
{
	if (msg.getParameters().size() < 2)
	{
		msg.getClient()->appendToSendBuffer(ERR_NEEDMOREPARAMS(msg.getCommand()));
		return 0;
	}

	Channel*	channel = getChannelByName(msg.getParameters()[0]);
	Client*		client = getClientByNickname(msg.getParameters()[1]);
	Client*		sender = msg.getClient();

	if (channel == NULL)
		return sender->appendToSendBuffer(ERR_NOSUCHCHANNEL(msg.getParameters()[0]));
	
	if (client == NULL)
		return sender->appendToSendBuffer(ERR_NOSUCHNICK(msg.getParameters()[1]));

	if (!channel->isOperator(sender))
		return sender->appendToSendBuffer(ERR_CHANOPRIVSNEEDED(channel->getName()));

	std::cout << "kicking " << client->getNickname() << " from channel " << channel->getName() << std::endl;

	channel->sendToChannel(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), "KICK", msg.getParameters(), msg.getTrailing()).toString());

	return 0;
}
