/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:21:39 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/26 11:58:29 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::quit(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	std::cout << "Client " << client->getNickname() << " quit" << std::endl;

	for (std::vector<Channel*>::const_iterator it = client->getChannels().begin(); it != client->getChannels().end(); ++it)
		(*it)->sendToChannel(IRCmsg(client, user_id(client->getNickname(), client->getUsername()), "QUIT", msg.getParameters(), msg.getTrailing()).toString());

	client->setToDisconnect(true);

	return 0;
}