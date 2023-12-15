/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/15 00:16:45 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

Channel::Channel(std::string name) : name(name)
{
	if (name.length() > CHANNEL_NAME_MAX_LENGTH)
		this->name = name.substr(0, CHANNEL_NAME_MAX_LENGTH);

	if (name[0] != '#')
		this->name = "#" + this->name;

	std::cout << "Channel " + this->name + " created" << std::endl;
}

Channel::~Channel() {std::cout << "Channel " + name + " destroyed" << std::endl;}

// getters
std::string	Channel::getName(void) const {return name;}
std::vector<Client*>	Channel::getClients(void) const {return clients;}

std::string	Channel::getNamesList(void)
{
	std::string	result;

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		result += (*it)->getNickname() + " ";

	return result;
}


void	Channel::addClient(Client* client)
{
	clients.push_back(client);
	client->addChannel(this);
}

void	Channel::removeClient(Client* client)
{
	std::vector<Client*>::iterator it = std::find(clients.begin(), clients.end(), client);

	if (it != clients.end())
	{
		std::cout << "Client " << client->getNickname() << " removed from channel " << name << std::endl;
		clients.erase(it);
	}
}
