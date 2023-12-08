/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:12:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/08 18:36:05 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

Client::Client(int	socket) : socket(socket) {}

Client::~Client() {}

bool	Client::operator==(const Client& other) const {return nickname == other.nickname;}

// getters
int		Client::getSocket() const {return socket;}
std::string	Client::getNickname() const {return nickname;}
std::string	Client::getUsername() const {return username;}
std::string	Client::getPassword() const {return password;}
std::string	Client::getRealname() const {return realname;}
std::string	Client::getMode() const {return mode;}

// setters
void	Client::setNickname(const std::string& nickname) {this->nickname = nickname;}
void	Client::setUsername(const std::string& username) {this->username = username;}
void	Client::setPassword(const std::string& password) {this->password = password;}
void	Client::setRealname(const std::string& realname) {this->realname = realname;}
