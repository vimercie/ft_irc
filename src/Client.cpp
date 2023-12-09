/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:12:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/09 01:18:13 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

Client::Client(int	socket) : socket(socket)
{
	cmds["NICK"] = &Client::nick;
	cmds["USER"] = &Client::user;
}

Client::~Client() {}

bool	Client::operator==(const Client& other) const {return nickname == other.nickname;}

// getters
int		Client::getSocket() const {return socket;}
std::string	Client::getNickname() const {return nickname;}
std::string	Client::getUsername() const {return username;}
std::string	Client::getHostname() const {return hostname;}
std::string	Client::getRealname() const {return realname;}
std::string	Client::getPassword() const {return password;}
std::string	Client::getMode() const {return mode;}

// setters
void	Client::setNickname(const std::string& nickname) {this->nickname = nickname;}
void	Client::setUsername(const std::string& username) {this->username = username;}
void	Client::setHostname(const std::string& hostname) {this->hostname = hostname;}
void	Client::setRealname(const std::string& realname) {this->realname = realname;}
void	Client::setPassword(const std::string& password) {this->password = password;}


// commands
void	Client::execCmd(const IRCmsg& msg)
{
	std::map<std::string, cmd>::iterator	it = cmds.find(msg.getCommand());

	if (it != cmds.end())
		(this->*(it->second))(msg);
}

void	Client::nick(const IRCmsg& msg)
{
	setNickname(msg.getParameters()[0]);
}

void	Client::user(const IRCmsg& msg)
{
	setUsername(msg.getParameters()[0]);
	setHostname(msg.getParameters()[1]);
	setRealname(msg.getTrailing());
}
