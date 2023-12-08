/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/08 19:33:17 by vimercie         ###   ########lyon.fr   */
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

void	Channel::addClient(int socket)
{
	Client* newClient = new Client(socket);

	while (true)
	{
		std::vector<IRCmsg*>	messages = server->readMsg(newClient->getSocket());

		for (std::vector<IRCmsg*>::iterator it = messages.begin(); it != messages.end(); it++)
		{
			if ((*it)->getCommand().empty())
				continue;

			(*it)->displayMessage();
			std::cout << std::endl;
		}
	}
}

// std::vector<Client>	Channel::getClients() const {return clients;}

// void	Channel::ircCmd(const IRCmsg& msg)
// {
// 	std::map<std::string, cmd>::iterator	it = cmds.find(msg.getCommand());

// 	if (it != cmds.end())
// 		(this->*(it->second))(msg);
// 	// else
// 	// 	std::cout << "Commande inconnue" << std::endl;
// }

// void	Channel::removeClient(const std::string& nickname)
// {
// 	std::vector<Client>::iterator	it = std::find(clients.begin(), clients.end(), nickname);

// 	if (it != clients.end())
// 		clients.erase(it);
// }

// void	Channel::welcome(const IRCmsg& msg)
// {
// 	IRCmsg	welcome;

// 	std::cout << "Welcome " << msg.getParameters()[0] << "!" << std::endl;
// 	welcome.setPrefix("irc.42.fr");
// 	welcome.setCommand("001");
// 	welcome.setParameters(std::vector<std::string>(1, msg.getParameters()[0]));
// 	welcome.setTrailing("Wesh wesh wesh " + msg.getParameters()[0]);

// 	std::cout << welcome.toString() << std::endl;
// }
