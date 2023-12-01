/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:24:18 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/01 17:29:44 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/IRCmsg.hpp"

IRCmsg::IRCmsg(const std::string& message)
{
	fromString(message);
}

IRCmsg::~IRCmsg() {}

void IRCmsg::fromString(const std::string& message)
{
	std::string::const_iterator it = message.begin();

	if (message[0] == ':')
		it = getNextWord(it, message, prefix);

	it = getNextWord(it, message, command);

	it = parseParameters(it, message);

	getNextWord(it, message, trailing);
}

std::string::const_iterator	IRCmsg::parseParameters(std::string::const_iterator& it, const std::string& message)
{
	std::string param;

	while (it != message.end())
	{
		// skip les espaces
		while (isspace(*it))
			it++;

		// si on arrive au trailing
		if (*it == ':')
			break ;

		it = getNextWord(it, message, param);

		// si on a pas de paramètre
		if (param.empty())
			break ;

		parameters.push_back(param);
	}

	return it;
}

std::string::const_iterator	IRCmsg::getNextWord(std::string::const_iterator& start, const std::string& message, std::string& buffer)
{
	// clear le buffer
	if (!buffer.empty())
		buffer.clear();

	// skip les espaces
	while (isspace(*start) && start != message.end())
		start++;

	// si on arrive à la fin du message
	if (start == message.end())
		return start;

    std::string::const_iterator end = start;

    while (!isspace(*end) && end != message.end())
        end++;

    buffer = std::string(start, end);

	return end;
}

void	IRCmsg::displayMessage()
{
	std::cout << "prefix = " << "|" << prefix << "|" << std::endl;
	std::cout << "command = " << "|" << command << "|" << std::endl;
	std::cout << "parameters = ";
	for (std::vector<std::string>::const_iterator it = parameters.begin(); it != parameters.end(); it++)
		std::cout << "|" << *it << "|" << " ";
	std::cout << std::endl;
	std::cout << "trailing = " << "|" << trailing << "|" << std::endl;
}
