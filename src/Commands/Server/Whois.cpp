/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 18:11:24 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/24 18:44:49 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

std::string	rpl_whoisuser(const std::string& nickname, const std::string& username, const std::string& hostname, const std::string& realname)
{
	// IRCmsg msg(":localhost", "311", nickname, username, hostname, "*", realname);
	IRCmsg						msg;
	std::vector<std::string>	params;

	params.push_back(nickname);
	params.push_back(username);
	params.push_back(hostname);
	params.push_back("*");

	std::cout << "realname: " << realname << std::endl;

	msg.setPrefix("localhost");
	msg.setCommand("311");
	msg.setParameters(params);
	msg.setTrailing(realname);

	std::cout << msg.toString() << std::endl;

	return msg.toString();
}

int	Server::whois(const IRCmsg& msg)
{
	Client*							sender = msg.getClient();
	const std::vector<std::string>&	params = msg.getParameters();

	if (params.size() < 1)
		return sender->appendToSendBuffer(ERR_NONICKNAMEGIVEN());

	Client* client = getClientByNickname(params[0]);

	if (client != NULL)
	{
		sender->appendToSendBuffer(RPL_WHOISUSER(client->getNickname(), client->getUsername(), client->getHostname(), client->getRealname()));
		return sender->appendToSendBuffer(RPL_ENDOFWHOIS(client->getNickname()));
	}

	return sender->appendToSendBuffer(ERR_NOSUCHNICK(params[0]));
}
