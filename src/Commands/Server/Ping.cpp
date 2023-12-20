/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:28:20 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/20 18:02:18 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/IRCmsg.hpp"

int	Server::ping(const IRCmsg& msg)
{
	Client*	client = msg.getClient();

	client->appendToSendBuffer(IRCmsg(client, "localhost", "PONG", msg.getParameters(), msg.getTrailing()).toString());

	return 0;
}