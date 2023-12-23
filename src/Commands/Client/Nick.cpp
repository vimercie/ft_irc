/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:16:55 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 16:32:00 by vimercie         ###   ########lyon.fr   */
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
	std::string	forbiddenChars = "[]\\`^{|}.,!?";

	// Si il n'y a pas de paramètre
	if (msg.getParameters().empty())
		return client->appendToSendBuffer(ERR_NONICKNAMEGIVEN());

	// Si il y a plus d'un paramètre
	if (msg.getParameters().size() > 1)
		return client->appendToSendBuffer(NOTICE("localhost", client->getNickname(), "Only one parameter needed"));

	// Si le nickname contient des caractères interdits
	if (msg.getParameters()[0].find_first_of(forbiddenChars) != std::string::npos)
		return client->appendToSendBuffer(ERR_ERRONEUSNICKNAME(msg.getParameters()[0]));

	// Si le nickname est déjà prit
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == msg.getParameters()[0])
			return client->appendToSendBuffer(ERR_NICKNAMEINUSE(msg.getParameters()[0]));
	}

	client->setNickname(msg.getParameters()[0]);

	return client->appendToSendBuffer(RPL_WELCOME(client->getNickname()));
}
