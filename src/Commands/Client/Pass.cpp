/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:15:14 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 16:30:41 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::pass(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	if (msg.getParameters().size() < 1)
	{
		client->setToDisconnect(true);
		return client->appendToSendBuffer(ERR_NEEDMOREPARAMS("PASS"));
	}

	if (msg.getParameters().size() > 1)
	{
		client->setToDisconnect(true);
		return client->appendToSendBuffer(NOTICE("localhost", client->getNickname(), "Only one parameter needed"));
	}

	if (client->isAuthenticated())
	{
		client->setToDisconnect(true);
		return client->appendToSendBuffer(ERR_ALREADYREGISTRED());
	}

	if (msg.getParameters()[0] != this->password)
	{
		client->setToDisconnect(true);
		return client->appendToSendBuffer(ERR_PASSWDMISMATCH());
	}

	client->setPass(true);

	return 0;
}
