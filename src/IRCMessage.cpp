/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:24:18 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/29 14:22:30 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/IRCMessage.hpp"

IRCMessage::IRCMessage(const std::string& message)
{
	parseMessage(message);
}

IRCMessage::~IRCMessage() {}

void IRCMessage::parseMessage(const std::string& message)
{
	std::string::const_iterator	start = message.begin();
	std::string::const_iterator	end = std::find(start, message.end(), ' ');

	if (*start == ':')
	{
		prefix = std::string(start + 1, end);
		start = end + 1;
		end = std::find(start, message.end(), ' ');
		std::cout << "Prefix: |" << prefix << "|" << std::endl;
	}

	command = std::string(start, end);
	std::cout << "Command: |" << command << "|" << std::endl;
}

