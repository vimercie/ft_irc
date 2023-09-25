/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/09/25 15:02:52 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/User.hpp"

Channel::Channel(std::string name)
{
	_name = parseName(name);

	_cmds["KICK"] = &Channel::kick;
	_cmds["INVITE"] = &Channel::invite;
	_cmds["TOPIC"] = &Channel::topic;
	_cmds["MODE"] = &Channel::mode;
}

Channel::~Channel() {}

void Channel::addUser(const User& user)
{
	if (std::find(_users.begin(), _users.end(), user) == _users.end())
		_users.push_back(User(user));
}

void Channel::removeUser(const User& user)
{
	std::vector<User>::iterator it = std::find(_users.begin(), _users.end(), user);
	if (it != _users.end())
		_users.erase(it);
}

std::string Channel::getName() const
{
	return (_name);
}

std::vector<User> Channel::getUsers() const
{
	return (_users);
}

std::string	Channel::parseName(const std::string& name)
{
	std::string res = name;

	if (res[0] != '#')
		res = "#" + res;
	if (res.size() > 200)
		res.resize(200);
	for (size_t i = 0; i < res.size(); i++)
	{
		if (isspace(res[i])
			|| res[i] == ','
			|| res[i] == 7
			|| res[i] == 0)
			res[i] = '_';
	}
	return (res);
}

// Commandes
void Channel::kick() {}

void Channel::invite() {}

void Channel::topic() {}

void Channel::mode() {}
