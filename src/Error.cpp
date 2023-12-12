/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:41:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/12 16:12:26 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

int	Server::err_passwdmismatch(const IRCmsg& msg)
{
	IRCmsg	response;

	response.setCommand("464");
	response.setPrefix("localhost");
	response.setParameters(std::vector<std::string>(1, msg.getClient()->getNickname()));
	response.setTrailing("Password incorrect");

	sendMsg(msg.getClient()->getSocket().fd, response.toString());

	return 1;
}
