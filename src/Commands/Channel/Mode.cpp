/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:30:49 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 17:15:03 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

int	Server::mode(const IRCmsg& msg)
{
	Channel* 						channel = getChannelByName(msg.getParameters()[0]);
	Client*							sender = msg.getClient();
	const std::vector<std::string>&	params = msg.getParameters();
	const std::string				validModes = "iotkl";

	if (params.size() < 1)
		return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS(msg.getCommand()));

	if (sender == NULL)
		return 0;

	if (channel == NULL)
	{
		if (params[0] != sender->getNickname())
			return sender->appendToSendBuffer(ERR_NOSUCHCHANNEL(params[0]));
		return (0);
	}

	if (params.size() == 1)
		return sender->appendToSendBuffer(RPL_CHANNELMODEIS(sender->getNickname(), channel->getName(), channel->getModes()));

	for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
	{
		const std::string&	param = *it;

		if (param.empty() || (param[0] != '+' && param[0] != '-'))
			continue;

		bool	modeSet = (param[0] == '+');

		for (size_t i = 1; i < param.length(); ++i)
		{
			if (std::find(validModes.begin(), validModes.end(), param[i]) == validModes.end())
			{
				sender->appendToSendBuffer(ERR_UNKNOWNMODE(param[i]));
				return 0;
			}

			if (param[i] == 'o' || param[i] == 'l' || param[i] == 'k')
			{
				if (!channel->isOperator(sender))
				{
					sender->appendToSendBuffer(ERR_CHANOPRIVSNEEDED(channel->getName()));
					return 0;
				}

				if (param.length() > 2)
				{
					sender->appendToSendBuffer(NOTICE("localhost", params[0], "Modes with parameters can only be set one at a time."));
					return 0;
				}

				if (param[i] == 'o')
				{
					if (params.size() < 3)
					{
						sender->appendToSendBuffer(ERR_NEEDMOREPARAMS("MODE"));
						return 0;
					}

					Client* target = getClientByNickname(params[1]);

					if (target == NULL)
					{
						sender->appendToSendBuffer(ERR_NOSUCHNICK(params[1]));
						return 0;
					}

					if (modeSet)
						channel->addOperator(target);
					else
						channel->removeOperator(target);
				}
				else if (param[i] == 'l')
				{
					if (params.size() < 3)
					{
						sender->appendToSendBuffer(ERR_NEEDMOREPARAMS("MODE"));
						return 0;
					}

					if (modeSet)
					{
						channel->setMode(param[i], modeSet);
						channel->setLimit(std::atoi(params[1].c_str()));
					}
					else
					{
						channel->setMode(param[i], modeSet);
						channel->setLimit(0);
					}
				}
				else if (param[i] == 'k')
				{
					msg.displayMessage();

					if (params.size() < 3)
					{
						sender->appendToSendBuffer(NOTICE("localhost", params[0], "You must specify a key."));
						return 0;
					}

					if (modeSet)
					{
						channel->setMode(param[i], modeSet);
						channel->setKey(params[1]);
					}
					else
					{
						channel->setMode(param[i], modeSet);
						channel->setKey("");
					}
				}
			}
			else
				channel->setMode(param[i], modeSet);
		}
	}

	return 0;
}
