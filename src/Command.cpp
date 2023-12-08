/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/08 18:56:08 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Command.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"

Command::Command(int type)
{
	userCmds["NICK"] = nick;
	userCmds["USER"] = user;
}

int	Command::nick(const IRCmsg& msg, Client* client, Channel* channel)
{
	
}

int	Command::user(const IRCmsg& msg, Client* client, Channel* channel)
{
	
}
