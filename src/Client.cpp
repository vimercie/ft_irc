/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:12:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/03 19:34:23 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(const std::string& nickname) : nickname(nickname) {}

Client::~Client() {}

bool	Client::operator==(const Client& other) const {return nickname == other.nickname;}


// setters
void	Client::setNickname(const std::string& nickname) {this->nickname = nickname;}

void	Client::setUsername(const std::string& username) {this->username = username;}

void	Client::setPassword(const std::string& password) {this->password = password;}

void	Client::setRealname(const std::string& realname) {this->realname = realname;}
