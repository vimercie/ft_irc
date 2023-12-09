/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/09 01:24:50 by vimercie         ###   ########lyon.fr   */
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

Channel::~Channel()
{
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		delete *it;
	clients.clear();

	std::cout << "Channel " + name + " destroyed" << std::endl;
}

void	Channel::addClient(Client* client)
{
	clients.push_back(client);
}
