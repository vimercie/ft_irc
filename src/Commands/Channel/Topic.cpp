/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:58:14 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/20 18:01:42 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::topic(const IRCmsg& msg)
{
	if (msg.getParameters().empty())
	{
		msg.getClient()->appendToSendBuffer(ERR_NEEDMOREPARAMS("TOPIC"));
		return 0;
	}

	Channel*	channel = getChannelByName(msg.getParameters()[0]);
	IRCmsg		response;

	if (channel == NULL)
		return 0;

	// if not operator and -t flag set
	if (channel->getMode('t') && !channel->isOperator(msg.getClient()))
		return 0;

	if (!msg.getTrailing().empty())
	{
		channel->setTopic(msg.getTrailing());
		std::cout << "topic of " << channel->getName() << " set to " << channel->getTopic() << std::endl;
	}

	response = IRCmsg(msg.getClient(), user_id(msg.getClient()->getNickname(), msg.getClient()->getUsername()), "TOPIC", msg.getParameters(), channel->getTopic());
	channel->sendToChannel(response.toString());

	return 0;
}