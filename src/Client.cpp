/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:12:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/06 16:22:31 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

Client::Client(int	socket)
{
	this->sock = socket;
}

Client::~Client() {}

bool	Client::operator==(const Client& other) const {return nickname == other.nickname;}


// setters
void	Client::setNickname(const std::string& nickname) {this->nickname = nickname;}

void	Client::setUsername(const std::string& username) {this->username = username;}

void	Client::setPassword(const std::string& password) {this->password = password;}

void	Client::setRealname(const std::string& realname)
{
	if (!realname.empty())
		this->realname = username;
}
