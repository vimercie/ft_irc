/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:24:18 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/22 17:37:22 by vimercie         ###   ########lyon.fr   */
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
    size_t start = 0;
    size_t end = message.find(' ');

    // Extract prefix (if present)
    if (message[0] == ':')
	{
        prefix = message.substr(1, end - 1);
        start = end + 1;
        end = message.find(' ', start);
    }

	std::cout << "TEEEST 1" << std::endl;

    // Extract command
    command = message.substr(start, end - start);

	std::cout << "TEEEST 2" << std::endl;

    // Extract parameters
    start = end + 1;
    while (start < message.length() && message[start] != ':')
	{
        end = message.find(' ', start);
        parameters.push_back(message.substr(start, end - start));
        start = end + 1;
    }

	std::cout << "TEEEST 3" << std::endl;

    // Extract trailing (if present)
    if (start < message.length())
	{
        trailing = message.substr(start + 1);
    }
}

void IRCMessage::displayParts() const
{
    std::cout << "Prefix: " << prefix << std::endl;
    std::cout << "Command: " << command << std::endl;

    std::cout << "Parameters: ";
    for (size_t i = 0; i < parameters.size(); ++i)
        std::cout << parameters[i] << " ";

    std::cout << std::endl;

    std::cout << "Trailing: " << trailing << std::endl;
}
