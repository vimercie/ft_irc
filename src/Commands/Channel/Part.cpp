/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:27:27 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/25 05:17:38 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::part(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();

	if (msg.getParameters().empty())
		return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS(msg.getCommand()));

	Channel*	channel = getChannelByName(msg.getParameters()[0]);

	if (channel == NULL || sender == NULL)
		return 0;

	channel->sendToChannel(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), "PART", msg.getParameters(), msg.getTrailing()).toString());
	channel->removeClient(sender);

	return 0;
}