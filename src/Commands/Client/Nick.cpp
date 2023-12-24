/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:16:55 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/24 18:03:29 by vimercie         ###   ########lyon.fr   */
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
	client->setNickname(getValidNickname(msg.getParameters()[0], client));

	return client->appendToSendBuffer(RPL_WELCOME(client->getNickname()));
}

std::string	Server::getValidNickname(const std::string& nickname, Client* client)
{
	std::ostringstream				res(nickname);
	Client*							tmp = getClientByNickname(res.str());
	short							i = 1;
	while (tmp != NULL && tmp != client)
	{
		res.str("");
		res << nickname << i;
		tmp = getClientByNickname(res.str());
		i++;
	}

	return res.str();
}
