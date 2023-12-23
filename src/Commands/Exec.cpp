/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exec.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:02:45 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 16:10:03 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/IRCmsg.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/RPL.hpp"

int Server::exec(const IRCmsg& msg)
{
	Client* client = msg.getClient();

	// Ignorer la commande CAP
	if (msg.getCommand() == "CAP")
		return 0;

	if (!client->isAuthenticated() && msg.getCommand() != "PASS")
	{
		std::cout << "Client " << client->getNickname() << " NOT authenticated" << std::endl;
		client->appendToSendBuffer(ERR_NEEDMOREPARAMS("PASS"));
		client->setToDisconnect(true);
		return 1;
	}

	std::map<std::string, cmd>::iterator it = cmds.find(msg.getCommand());

	if (it != cmds.end())
		(this->*(it->second))(msg);
	else
		client->appendToSendBuffer(ERR_UNKNOWNCOMMAND(msg.getCommand()));

	client->clearRecvBuffer();

	return 0;
}