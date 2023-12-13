/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:12:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/13 04:58:50 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

Client::Client(pollfd	*socket) : socket(socket), pass(false) {std::cout << "Client created (fd: " << socket->fd << ")" << std::endl;}

Client::~Client() {std::cout << "Client destroyed" << std::endl;}

bool	Client::operator==(const Client& other) const {return nickname == other.nickname;}

// getters
pollfd		Client::getSocket() const {return *socket;}
std::string	Client::getNickname() const {return nickname;}
std::string	Client::getUsername() const {return username;}
std::string	Client::getHostname() const {return hostname;}
std::string	Client::getRealname() const {return realname;}
std::string	Client::getMode() const {return mode;}
std::vector<Channel*>	Client::getChannels(void) {return channels;}
bool		Client::isAuthenticated() const {return pass;}

// setters
void	Client::setNickname(const std::string& nickname) {this->nickname = nickname;}
void	Client::setUsername(const std::string& username) {this->username = username;}
void	Client::setHostname(const std::string& hostname) {this->hostname = hostname;}
void	Client::setRealname(const std::string& realname) {this->realname = realname;}
void	Client::setMode(const std::string& mode) {this->mode = mode;}
void	Client::setPass(bool pass) {this->pass = pass;}

// methods
void	Client::addChannel(Channel* channel)
{
	channels.push_back(channel);
}

void	Client::removeChannel(Channel* channel)
{
	std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);

	if (it != channels.end())
		channels.erase(it);
}