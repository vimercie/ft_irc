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
# include "../inc/Utils.hpp"

IRCmsg::IRCmsg() {}

IRCmsg::IRCmsg(Client* client, const std::string& message) : client(client)
{
	fromString(message);
}

IRCmsg::IRCmsg(Client* client, const std::string& prefix, const std::string& command, const std::vector<std::string>& parameters, const std::string& trailing)
{
	this->client = client;

	if (!prefix.empty())
		this->prefix = prefix;

	this->command = command;

	if (!parameters.empty())
	this->parameters = parameters;

	if (!trailing.empty())
		this->trailing = trailing;
}

IRCmsg::IRCmsg(const IRCmsg& other)
{
	*this = other;
}

IRCmsg::~IRCmsg() {}

IRCmsg& IRCmsg::operator=(const IRCmsg& other)
{
	if (this == &other)
		return *this;

	client = other.client;
	prefix = other.prefix;
	command = other.command;
	parameters = other.parameters;
	trailing = other.trailing;

	return *this;
}

// getters
Client* IRCmsg::getClient() const {return client;}
std::string IRCmsg::getPrefix() const {return prefix;}
std::string IRCmsg::getCommand() const {return command;}
std::vector<std::string> IRCmsg::getParameters() const {return parameters;}
std::string IRCmsg::getTrailing() const {return trailing;}


// setters
void IRCmsg::setPrefix(const std::string& prefix)
{
	if (prefix[0] == ':')
		this->prefix = prefix.substr(1);
	else
		this->prefix = prefix;
}
void IRCmsg::setCommand(const std::string& command) {this->command = command;}
void IRCmsg::setParameters(const std::vector<std::string>& parameters) {this->parameters = parameters;}
void IRCmsg::setTrailing(const std::string& trailing) {this->trailing = trailing;}
void IRCmsg::setClient(Client* client) {this->client = client;}


// Conversion de la string en IRCmsg
void IRCmsg::fromString(const std::string& message)
{
	std::string::const_iterator it = message.begin();

	if (message[0] == ':')
		it = getNextWord(it, message, prefix);

	it = getNextWord(it, message, command);

	it = parseParameters(it, message);

	parseTrailing(it, message);
}

std::string IRCmsg::toString() const
{
	std::string message;

	if (!prefix.empty())
		message += ":" + prefix + " ";

	message += command;

	if (!parameters.empty())
		message += " ";
	for (std::vector<std::string>::const_iterator it = parameters.begin(); it != parameters.end(); it++)
	{
		message += *it;
		if (it + 1 != parameters.end())
			message += " ";
	}

	if (!trailing.empty())
		message += " " + trailing;

	message += "\r\n";

	return message;
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
		param.clear();
	}

	return it;
}

std::string::const_iterator	IRCmsg::parseTrailing(std::string::const_iterator& it, const std::string& message)
{
	std::string res;
	std::string nospcrlfcl = ":\r\n";

	if (*it == ':')
	{
		res += *it;
		it++;
	}

	while (it != message.end())
	{
		if (nospcrlfcl.find(*it) != std::string::npos)
			break ;

		res += *it;
		it++;
	}

	setTrailing(res);

	return it;
}

void	IRCmsg::displayMessage() const
{
	std::cout << "prefix = " << "|" << prefix << "|" << std::endl;
	std::cout << "command = " << "|" << command << "|" << std::endl;
	std::cout << "parameters = ";
	for (std::vector<std::string>::const_iterator it = parameters.begin(); it != parameters.end(); it++)
		std::cout << "|" << *it << "|" << " ";
	std::cout << std::endl;
	std::cout << "trailing = " << "|" << trailing << "|" << std::endl;
	std::cout << "param size = " << parameters.size() << std::endl;
}
