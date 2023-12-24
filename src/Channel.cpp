/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/24 16:20:00 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

Channel::Channel(const std::string& name) : name(name)
{
	if (name.length() > CHANNEL_NAME_MAX_LENGTH)
		this->name = name.substr(0, CHANNEL_NAME_MAX_LENGTH);

	if (name[0] != '#')
		this->name = "#" + this->name;
	
	std::cout << "Channel " + this->name + " created" << std::endl;
	//set modes
	modes['i'] = false;
	modes['t'] = true;
	modes['k'] = false;
	modes['l'] = false;
	limit = 0;
	key = "";
}

Channel::~Channel() {std::cout << "Channel " + name + " destroyed" << std::endl;}

// getters
std::string					Channel::getName(void) const {return name;}
const std::vector<Client*>&	Channel::getClients() const { return clients; }
std::string					Channel::getTopic(void) const {return topic;}
std::string					Channel::getKey(void) const {return key;}
unsigned int				Channel::getLimit(void) const {return limit;}
bool						Channel::isOperator(Client* client) const {return std::find(operators.begin(), operators.end(), client) != operators.end();}
bool						Channel::isInvited(Client* client) const {return std::find(invited.begin(), invited.end(), client) != invited.end();}
bool						Channel::getMode(char mode) const
{
	std::map<char, bool>::const_iterator it = modes.find(mode);

	if (it == modes.end())
		return false;
	return it->second;
}
std::string	Channel::getModes(void) const
{
	std::string result;

	for (std::map<char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (it->second)
			result += it->first;
	}

	return result;
}
std::string	Channel::getNamesList(void)
{
	std::string	result;

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		result += (*it)->getNickname() + " ";

	return result;
}


// setters
void	Channel::setName(const std::string& name) {this->name = name;}
void	Channel::setTopic(const std::string& topic) {this->topic = topic;}
void	Channel::setKey(const std::string& key) {this->key = key;}
void	Channel::setMode(char mode, bool value) {modes[mode] = value;}
void	Channel::setLimit(unsigned int limit) {this->limit = limit;}

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
		client->removeChannel(this);
	}
}

void	Channel::addOperator(Client* client)
{
	// if already operator
	if (isOperator(client))
		return;

	operators.push_back(client);
}

void	Channel::removeOperator(Client* client)
{
	// if not operator
	if (!isOperator(client))
		return;
	std::vector<Client*>::iterator it = std::find(operators.begin(), operators.end(), client);

	if (it != operators.end())
	{
		std::cout << "Client " << client->getNickname() << " removed from operators of channel " << name << std::endl;
		operators.erase(it);
	}
}

void	Channel::sendToChannel(const std::string& msg)
{
	const std::vector<Client*>& clients = getClients();

	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		Client* client = *it;

		if (!client)
			continue;

		client->appendToSendBuffer(msg);
	}
}

void	Channel::sendInvite(Client* client)
{
	if (isInvited(client))
		return;
	invited.push_back(client);
}