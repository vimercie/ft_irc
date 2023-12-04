/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/03 20:20:24 by vimercie         ###   ########lyon.fr   */
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

	cmds["NICK"] = &Channel::nick;
	cmds["USER"] = &Channel::user;
	// cmds["JOIN"] = &Channel::join;
	// cmds["KICK"] = &Channel::kick;
	// cmds["INVITE"] = &Channel::invite;
	// cmds["TOPIC"] = &Channel::topic;
	// cmds["MODE"] = &Channel::mode;
}

Channel::~Channel() {}

void	Channel::ircCmd(const IRCmsg& msg)
{
	std::map<std::string, cmd>::iterator	it = cmds.find(msg.getCommand());

	if (it != cmds.end())
		(this->*(it->second))(msg);
	else
		std::cout << "Commande inconnue" << std::endl;
}

void	Channel::addClient(const std::string& nickname)
{
	clients.push_back(Client(nickname));
}

void	Channel::welcome(const IRCmsg& msg)
{
	IRCmsg	welcome;

	std::cout << "Welcome " << msg.getParameters()[0] << "!" << std::endl;
	welcome.setPrefix("irc.42.fr");
	welcome.setCommand("001");
	welcome.setParameters(std::vector<std::string>(1, msg.getParameters()[0]));
	welcome.setTrailing("Wesh wesh wesh " + msg.getParameters()[0] + "!" + msg.getParameters()[3]);

	std::cout << welcome.toString() << std::endl;
}

void	Channel::nick(const IRCmsg& msg)
{
	if (msg.getParameters().size() != 1
		|| msg.getParameters()[0].empty()
		|| msg.getParameters()[0].length() > NICKNAME_MAX_LENGTH)
		return ;

	std::vector<Client>::iterator	it = std::find(clients.begin(), clients.end(), msg.getParameters()[0]);

	if (it == clients.end())
		addClient(msg.getParameters()[0]);
	else
		it->setNickname(msg.getParameters()[0]);
}

void	Channel::user(const IRCmsg& msg)
{
	// std::cout << "TEEEEEEST" << std::endl;
	// std::vector<Client>::iterator	it = std::find(clients.begin(), clients.end(), msg.getParameters()[0]);
	// std::cout << "TEEEEEEST" << std::endl;

	// if (it == clients.end())
	// {
	// 	std::cout << "TEEEEEEST" << std::endl;
	// 	return ;
	// }
	// else
	// {
	// 	it->setUsername(msg.getParameters()[0]);
	// 	it->setRealname(msg.getParameters()[3]);
	// }

	welcome(msg);
}

