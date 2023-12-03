/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:08:59 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/03 15:09:06 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"

Channel::Channel(std::string name) : name(name) {}

Channel::~Channel() {}

void	Channel::addClient(const std::string& nickname)
{
	clients.push_back(Client(nickname));
}
