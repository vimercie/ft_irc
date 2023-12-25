/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:58:14 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/25 04:45:13 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::topic(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();

	if (msg.getParameters().empty())
		return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS(msg.getCommand()));

	Channel*	channel = getChannelByName(msg.getParameters()[0]);

	if (channel == NULL)
		return sender->appendToSendBuffer(ERR_NOSUCHCHANNEL(msg.getParameters()[0]));

	// if not operator and -t flag set
	if (channel->getMode('t') && !channel->isOperator(sender))
		return sender->appendToSendBuffer(ERR_CHANOPRIVSNEEDED(channel->getName()));

	if (!msg.getTrailing().empty())
	{
		channel->setTopic(msg.getTrailing());
		std::cout << "topic of " << channel->getName() << " set to " << channel->getTopic() << std::endl;
	}

	channel->sendToChannel(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), msg.getCommand(), msg.getParameters(), channel->getTopic()).toString());

	return 0;
}
