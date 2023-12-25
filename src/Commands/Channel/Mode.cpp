/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 17:30:49 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/25 04:54:13 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Server.hpp"
#include "../../../inc/Client.hpp"
#include "../../../inc/Channel.hpp"
#include "../../../inc/IRCmsg.hpp"
#include "../../../inc/RPL.hpp"

typedef int	(Server::*ServerMode)(Channel*, Client*, const std::string&, bool);

int	Server::modeI(Channel* channel, Client* sender, const std::string& param, bool modeSet)
{
	(void)param;
	(void)sender;
	channel->setMode('i', modeSet);
	return 0;
}

int	Server::modeO(Channel* channel, Client* sender, const std::string& param, bool modeSet)
{
	if (param.empty())
		return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS("MODE"));

	Client*	target = getClientByNickname(param);

	if (target == NULL)
		return sender->appendToSendBuffer(ERR_NOSUCHNICK(param));

	if (modeSet)
		channel->addOperator(target);
	else
		channel->removeOperator(target);

	return 0;
}

int	Server::modeT(Channel* channel, Client* sender, const std::string& param, bool modeSet)
{
	(void)param;
	(void)sender;
	channel->setMode('t', modeSet);
	return 0;
}

int	Server::modeK(Channel* channel, Client* sender, const std::string& param, bool modeSet)
{
	if (modeSet && param.empty())
		return sender->appendToSendBuffer(NOTICE("localhost", channel->getName(), "You must specify a key."));

	if (modeSet)
		channel->setKey(param);
	else
		channel->setKey("");

	channel->setMode('k', modeSet);

	return 0;
}

int	Server::modeL(Channel* channel, Client* sender, const std::string& param, bool modeSet)
{
	if (modeSet && param.empty())
		return sender->appendToSendBuffer(NOTICE("localhost", channel->getName(), "You must specify a limit."));

	channel->setMode('l', modeSet);

	if (modeSet)
	{
		// Si la limite m'est pas composée que de chiffres
		if (param.find_first_not_of("0123456789") != std::string::npos)
			return sender->appendToSendBuffer(NOTICE("localhost", channel->getName(), "The limit must be a positive number."));

		size_t	limit = std::atoi(param.c_str());

		if (limit < channel->getClients().size())
			return sender->appendToSendBuffer(NOTICE("localhost", channel->getName(), "The limit cannot be lower than the number of clients in the channel."));

		channel->setLimit(limit);
	}
	else
		channel->setLimit(0);

	return 0;
}

bool isFormattedCorrectly(const std::string& str)
{
    std::istringstream iss(str);
    char sign, letter;

    // Extraire les caractères
    if (!(iss >> sign >> letter))
        return false; // Échec de l'extraction des caractères

    // Vérifier s'il y a des caractères supplémentaires
    char extra;
    if (iss >> extra)
        return false;

    // Vérifier le signe
    if (sign != '+' && sign != '-')
        return false;

    // Vérifier la lettre
    if (letter != 'i' && letter != 'o' && letter != 't' && letter != 'k' && letter != 'l')
        return false;

    return true; // Le format est correct
}

int	Server::mode(const IRCmsg& msg)
{
	Client*								sender = msg.getClient();
	const std::vector<std::string>&		params = msg.getParameters();

	if (params.empty())
		return sender->appendToSendBuffer(ERR_NEEDMOREPARAMS(msg.getCommand()));

	if (params.size() > 3)
		return sender->appendToSendBuffer(NOTICE("localhost", sender->getNickname(), "Too many parameters."));

	// Les modes d'utilisateur ne sont pas implémentés
	if (params[0][0] != '#')
		return sender->appendToSendBuffer(NOTICE("localhost", sender->getNickname(), "User modes are not implemented."));

	Channel*	channel = getChannelByName(msg.getParameters()[0]);

	// Si le channel n'existe pas
	if (channel == NULL)
		return sender->appendToSendBuffer(ERR_NOSUCHCHANNEL(params[0]));

	// Si il n'y a qu'un paramètre, on affiche les modes du channel
	if (params.size() == 1)
		return sender->appendToSendBuffer(RPL_CHANNELMODEIS(sender->getNickname(), channel->getName(), channel->getModes()));

	// Si l'utilisateur n'est pas opérateur
	if (!channel->isOperator(sender))
		return sender->appendToSendBuffer(ERR_CHANOPRIVSNEEDED(channel->getName()));

	if (!isFormattedCorrectly(params[1]))
		return sender->appendToSendBuffer(ERR_UMODEUNKNOWNFLAG());

	std::map<std::string, ServerMode>	modeList;
	bool								modeSet = (params[1][0] == '+');
	char								mode = params[1][1];

	modeList["i"] = &Server::modeI;
	modeList["o"] = &Server::modeO;
	modeList["t"] = &Server::modeT;
	modeList["k"] = &Server::modeK;
	modeList["l"] = &Server::modeL;

	if (modeList.find(std::string(1, mode)) == modeList.end())
		return sender->appendToSendBuffer(ERR_UNKNOWNMODE(mode));

	std::string	modeParam;

	if (params.size() == 3)
		modeParam = params[2];

	return (this->*modeList[std::string(1, mode)])(channel, sender, modeParam, modeSet);

	// On parcourt la liste des paramètres
	// for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
	// {
	// 	const std::string&	param = *it;

	// 	if (param.empty() || (param[0] != '+' && param[0] != '-'))
	// 		continue;

	// 	bool	modeSet = (param[0] == '+');

	// 	for (size_t i = 1; i < param.length(); ++i)
	// 	{
	// 		if (std::find(validModes.begin(), validModes.end(), param[i]) == validModes.end())
	// 		{
	// 			sender->appendToSendBuffer(ERR_UNKNOWNMODE(param[i]));
	// 			return 0;
	// 		}

	// 		if (param[i] == 'o' || param[i] == 'l' || param[i] == 'k')
	// 		{
	// 			if (!channel->isOperator(sender))
	// 			{
	// 				sender->appendToSendBuffer(ERR_CHANOPRIVSNEEDED(channel->getName()));
	// 				return 0;
	// 			}

	// 			if (param.length() > 2)
	// 			{
	// 				sender->appendToSendBuffer(NOTICE("localhost", params[0], "Modes with parameters can only be set one at a time."));
	// 				return 0;
	// 			}

	// 			if (param[i] == 'o')
	// 			{
	// 				if (params.size() < 3)
	// 				{
	// 					sender->appendToSendBuffer(ERR_NEEDMOREPARAMS("MODE"));
	// 					return 0;
	// 				}

	// 				Client* target = getClientByNickname(params[1]);

	// 				if (target == NULL)
	// 				{
	// 					sender->appendToSendBuffer(ERR_NOSUCHNICK(params[1]));
	// 					return 0;
	// 				}

	// 				if (modeSet)
	// 					channel->addOperator(target);
	// 				else
	// 					channel->removeOperator(target);
	// 			}
	// 			else if (param[i] == 'l')
	// 			{
	// 				if (params.size() < 3)
	// 				{
	// 					sender->appendToSendBuffer(ERR_NEEDMOREPARAMS("MODE"));
	// 					return 0;
	// 				}

	// 				if (modeSet)
	// 				{
	// 					channel->setMode(param[i], modeSet);
	// 					channel->setLimit(std::atoi(params[1].c_str()));
	// 				}
	// 				else
	// 				{
	// 					channel->setMode(param[i], modeSet);
	// 					channel->setLimit(0);
	// 				}
	// 			}
	// 			else if (param[i] == 'k')
	// 			{
	// 				msg.displayMessage();

	// 				if (params.size() < 3)
	// 				{
	// 					sender->appendToSendBuffer(NOTICE("localhost", params[0], "You must specify a key."));
	// 					return 0;
	// 				}

	// 				if (modeSet)
	// 				{
	// 					channel->setMode(param[i], modeSet);
	// 					channel->setKey(params[1]);
	// 				}
	// 				else
	// 				{
	// 					channel->setMode(param[i], modeSet);
	// 					channel->setKey("");
	// 				}
	// 			}
	// 		}
	// 		else
	// 			channel->setMode(param[i], modeSet);
	// 	}
	// }

	return 0;
}
