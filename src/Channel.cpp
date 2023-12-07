/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/06 16:28:16 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"

Channel::Channel(std::string name) : name(name)
{
	if (name[0] != '#')
		name = "#" + name;

	if (name.length() > CHANNEL_NAME_MAX_LENGTH)
		name = name.substr(0, CHANNEL_NAME_MAX_LENGTH);
}

Channel::~Channel() {}

std::vector<Client>	Channel::getClients() const {return clients;}

void	Channel::ircCmd(const IRCmsg& msg)
{
	std::map<std::string, cmd>::iterator	it = cmds.find(msg.getCommand());

	if (it != cmds.end())
		(this->*(it->second))(msg);
	// else
	// 	std::cout << "Commande inconnue" << std::endl;
}

void	Channel::addClient(const Client &client)
{
	clients.push_back(client);
}

// void	Channel::removeClient(const std::string& nickname)
// {
// 	std::vector<Client>::iterator	it = std::find(clients.begin(), clients.end(), nickname);

// 	if (it != clients.end())
// 		clients.erase(it);
// }

void	Channel::welcome(const IRCmsg& msg)
{
	IRCmsg	welcome;

	std::cout << "Welcome " << msg.getParameters()[0] << "!" << std::endl;
	welcome.setPrefix("irc.42.fr");
	welcome.setCommand("001");
	welcome.setParameters(std::vector<std::string>(1, msg.getParameters()[0]));
	welcome.setTrailing("Wesh wesh wesh " + msg.getParameters()[0]);

	std::cout << welcome.toString() << std::endl;
}

