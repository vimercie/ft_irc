/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:16:55 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/21 10:54:28 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

// user related commands
int	Server::nick(const IRCmsg& msg)
{
	Client		*client = msg.getClient();
	std::string	forbiddenChars = "[]\\`_^{|}";

	// Si le nickname est déjà prit
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == msg.getParameters()[0])
		{
			client->appendToSendBuffer(ERR_NICKNAMEINUSE(msg.getParameters()[0]));
			client->setToDisconnect(true);
			return 1;
		}
	}

	if (msg.getParameters()[0].find_first_of(forbiddenChars) != std::string::npos)
	{
		client->appendToSendBuffer(ERR_ERRONEUSNICKNAME(msg.getParameters()[0]));
		client->setToDisconnect(true);
		return 1;
	}

	if (!client->isAuthenticated())
	{
		std::cout << "Client " << client->getNickname() << " NOT authenticated" << std::endl;
		client->appendToSendBuffer(ERR_NEEDMOREPARAMS("PASS"));
		client->setToDisconnect(true);
		return 1;
	}

	client->setNickname(msg.getParameters()[0]);

	client->appendToSendBuffer(RPL_WELCOME(client->getNickname()));

	return 0;
}
