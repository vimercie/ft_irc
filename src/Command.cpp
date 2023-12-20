/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:38:54 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/20 07:59:14 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCmsg.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Utils.hpp"
#include "../inc/RPL.hpp"

int Server::exec(const IRCmsg& msg)
{
	std::map<std::string, cmd>::iterator it = cmds.find(msg.getCommand());

	if (it != cmds.end())
		return (this->*(it->second))(msg);
	else
		std::cout << "Command not found" << std::endl;

	std::cout << std::endl;

	return 0;
}

// user related commands
int	Server::nick(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	if (!client->isAuthenticated())
	{
		std::cout << "Client " << client->getNickname() << " NOT authenticated" << std::endl;
		client->appendToSendBuffer(ERR_NEEDMOREPARAMS("PASS"));
		client->setToDisconnect(true);
		return 1;
	}

	client->setNickname(msg.getParameters()[0]);

	client->appendToSendBuffer(RPL_WELCOME(client->getNickname()));

	return 0;
}

int	Server::user(const IRCmsg& msg)
{
	msg.getClient()->setUsername(msg.getParameters()[0]);
	msg.getClient()->setHostname(msg.getParameters()[1]);
	msg.getClient()->setRealname(msg.getTrailing());

	return 0;
}

int	Server::pass(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	if (msg.getParameters()[0] != this->password)
	{
		client->appendToSendBuffer(ERR_PASSWDMISMATCH());
		client->setToDisconnect(true);
		return 1;
	}

	client->setPass(true);

	return 0;
}

int	Server::quit(const IRCmsg& msg)
{
	Client	*client = msg.getClient();

	std::cout << "Client " << client->getNickname() << " quit (" << msg.getTrailing() << ")" << std::endl;
	client->setToDisconnect(true);

	return 0;
}

// channel related commands

int	Server::join(const IRCmsg& msg)
{
	Channel*	channel= getChannelByName(msg.getParameters()[0]);
	Client*		sender = msg.getClient();

	if (channel == NULL)
	{
		channel = new Channel(msg.getParameters()[0]);
		channels.push_back(channel);
		channel->addOperator(sender);
	}

	//if channel limit reached
	if (channel->getLimit() != 0 && channel->getClients().size() + 1 > channel->getLimit())
	{
		sender->appendToSendBuffer(ERR_CHANNELISFULL(channel->getName()));
		return 0;
	}
	//if channel is invite only
	if (channel->getMode('i') && !channel->isInvited(sender))
	{
		sender->appendToSendBuffer(ERR_INVITEONLYCHAN(channel->getName()));
		return 0;
	}
	//if channel is password protected
	if (channel->getMode('k') && channel->getKey() != msg.getParameters()[1])
	{
		sender->appendToSendBuffer(ERR_BADCHANNELKEY(channel->getName()));
		return 0;
	}

	channel->addClient(sender);

	if (channel->getTopic().empty())
		sender->appendToSendBuffer(RPL_NOTOPIC(channel->getName()));
	else
		sender->appendToSendBuffer(RPL_TOPIC(sender->getNickname() , channel->getName(), channel->getTopic()));
	return 0;
}

int Server::privmsg(const IRCmsg& msg)
{
	Client				*sender = msg.getClient();
	const std::string	recipient = msg.getParameters()[0];

	if (!sender)
		return 0;

	if (recipient[0] != '#')
	{
		Client* client = getClientByNickname(recipient);

		if (client && client != sender)
			return privmsgToClient(msg, client);
	}

	Channel* channel = getChannelByName(recipient);

	if (!channel)
	{
		sender->appendToSendBuffer(ERR_NOSUCHNICK(recipient));
		return 0;
	}

	IRCmsg response(msg);
	response.setPrefix(user_id(sender->getNickname(), sender->getUsername()));

	return privmsgToChannel(response, channel);
}

int	Server::topic(const IRCmsg& msg)
{
	if (msg.getParameters().empty())
	{
		msg.getClient()->appendToSendBuffer(ERR_NEEDMOREPARAMS("TOPIC"));
		return 0;
	}

	Channel*	channel = getChannelByName(msg.getParameters()[0]);
	IRCmsg		response;

	if (channel == NULL)
		return 0;

	// if not operator and -t flag set
	if (channel->getMode('t') && !channel->isOperator(msg.getClient()))
		return 0;

	if (!msg.getTrailing().empty())
	{
		channel->setTopic(msg.getTrailing());
		std::cout << "topic of " << channel->getName() << " set to " << channel->getTopic() << std::endl;
	}

	response = IRCmsg(msg.getClient(), user_id(msg.getClient()->getNickname(), msg.getClient()->getUsername()), "TOPIC", msg.getParameters(), channel->getTopic());
	channel->sendToChannel(response.toString());

	return 0;
}

// int	Server::mode(const IRCmsg& msg)
// {
// 	Channel*	channel		= getChannelByName(msg.getParameters()[0]);

// 	if (channel == NULL)
// 	{
// 		std::cout << "channel not found" << std::endl;
// 		return 0;
// 	}

// 	// if asked for channel modes (no permission needed)
// 	if (msg.getParameters().size() == 1 && msg.getParameters()[0] == channel->getName())
// 	{
// 		std::cout << "channel modes: " << channel->getModes() << std::endl;
// 		msg.getClient()->appendToSendBuffer(RPL_CHANNELMODEIS(channel->getName(), channel->getModes()));
// 		return 0;
// 	}

// 	// if not operator
// 	if (!channel->isOperator(msg.getClient()))
// 	{
// 		std::cout << "not operator" << std::endl;
// 		//send message to client
// 		msg.getClient()->appendToSendBuffer("You are not operator");
// 		return 0;
// 	}

// 	// if mode +o or -o
// 	msg.displayMessage();
// 	if (msg.getParameters().size() == 3 && msg.getParameters()[1][0] == 'o')
// 	{
// 		std::cout << "setting mode o for channel " << channel->getName() << std::endl;
// 		Client* client = getClientByNickname(msg.getParameters()[2]);

// 		if (client == NULL)
// 			return 0;

// 		if (msg.getParameters()[0] != client->getNickname())
// 			return 0;

// 		if (msg.getParameters()[0] == client->getNickname() && msg.getParameters()[1] == "+o")
// 			channel->addOperator(client);
// 		else if (msg.getParameters()[0] == client->getNickname() && msg.getParameters()[1] == "-o")
// 			channel->removeOperator(client);
// 		return 0;
// 	}

// 	// if mode +l or -l
// 	if (msg.getParameters().size() == 3 && msg.getParameters()[1][0] == 'l')
// 	{
// 		std::cout << "setting mode l for channel " << channel->getName() << std::endl;
// 		if (msg.getParameters()[0] != channel->getName())
// 			return 0;

// 		if (msg.getParameters()[0] == channel->getName() && msg.getParameters()[1] == "+l")
// 		{
// 			channel->setMode('l', true);
// 			channel->setLimit(std::atoi(msg.getParameters()[2].c_str()));
// 		}
// 		else if (msg.getParameters()[0] == channel->getName() && msg.getParameters()[1] == "-l")
// 		{
// 			channel->setMode('l', false);
// 			channel->setLimit(0);
// 		}
// 		return 0;
// 	}

// 	// if mode +k or -k
// 	if (msg.getParameters().size() == 3 && msg.getParameters()[1][0] == 'k')
// 	{
// 		std::cout << "setting mode k for channel " << channel->getName() << std::endl;
// 		if (msg.getParameters()[0] != channel->getName())
// 			return 0;

// 		if (msg.getParameters()[0] == channel->getName() && msg.getParameters()[1] == "+k")
// 		{
// 			channel->setMode('k', true);
// 			channel->setKey(msg.getParameters()[2]);
// 		}
// 		else if (msg.getParameters()[0] == channel->getName() && msg.getParameters()[1] == "-k")
// 		{
// 			channel->setMode('k', false);
// 			channel->setKey("");
// 		}
// 		return 0;
// 	}

// 	// if mode +i or -i
// 	if (msg.getParameters().size() == 2 && msg.getParameters()[1][1] == 'i')
// 	{
// 		if (msg.getParameters()[0] != channel->getName())
// 		{
// 			std::cout << "channel name not matching" << std::endl;
// 			return 0;
// 		}
// 		if (msg.getParameters()[0] == channel->getName() && msg.getParameters()[1] == "+i")
// 		{
// 			std::cout << "setting mode i for channel " << channel->getName() << std::endl;
// 			channel->setMode('i', true);
// 		}
// 		else if (msg.getParameters()[0] == channel->getName() && msg.getParameters()[1] == "-i")
// 			channel->setMode('i', false);
// 		return 0;
// 	}
// 	return 0;
// }

int	Server::part(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();
	Channel*	channel = getChannelByName(msg.getParameters()[0]);

	if (channel == NULL)
		return 0;

	channel->sendToChannel(IRCmsg(sender, user_id(sender->getNickname(), sender->getUsername()), "PART", msg.getParameters(), msg.getTrailing()).toString());
	channel->removeClient(msg.getClient());
	sender->removeChannel(channel);


	return 0;
}

// server related commands
int	Server::ping(const IRCmsg& msg)
{
	Client*	client = msg.getClient();

	client->appendToSendBuffer(IRCmsg(client, "localhost", "PONG", msg.getParameters(), msg.getTrailing()).toString());

	return 0;
}

// utils
int Server::privmsgToChannel(const IRCmsg& msg, Channel* channel)
{
	std::vector<Client*>	clients = channel->getClients();
	Client* 				sender = msg.getClient();

	if (std::find(clients.begin(), clients.end(), sender) == clients.end())
	{
		std::cout << "Client " << sender->getNickname() << " not in channel " << channel->getName() << std::endl;
		sender->appendToSendBuffer(ERR_NOTONCHANNEL(channel->getName()));
		return 0;
	}

   	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		Client* client = *it;

		if (!client || client == sender)
			continue;

		client->appendToSendBuffer(msg.toString());
	}

	return 0;
}

int Server::privmsgToClient(const IRCmsg& msg, Client* recipient)
{
	const Client* sender = msg.getClient();

	IRCmsg response;
	response.setClient(recipient);
	response.setPrefix(user_id(sender->getNickname(), sender->getUsername()));
	response.setCommand("PRIVMSG");
	response.setParameters(std::vector<std::string>(1, recipient->getNickname()));
	response.setTrailing(msg.getTrailing());

	recipient->appendToSendBuffer(response.toString());

	return 0;
}

int	Server::invite(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();
	Client*		client = getClientByNickname(msg.getParameters()[0]);
	Channel*	channel = getChannelByName(msg.getParameters()[1]);

	if (client == NULL || channel == NULL)
		return 0;

	if (!channel->isOperator(sender))
		return 0;

	// if client already invited
	if (channel->isInvited(client))
		return 0;

	channel->sendInvite(client);
	client->appendToSendBuffer(RPL_INVITING(client->getNickname(), channel->getName()));
	client->appendToSendBuffer(RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));

	return 0;
}

int Server::kick(const IRCmsg& msg)
{
	Client*		sender = msg.getClient();
	Client*		client = getClientByNickname(msg.getParameters()[1]);
	Channel*	channel = getChannelByName(msg.getParameters()[0]);

	if (client == NULL || channel == NULL)
	{
		std::cout << "client or channel not found" << std::endl;
		return 0;
	}

	if (!channel->isOperator(sender))
	{
		std::cout << sender->getNickname() << " is not operator of channel " << channel->getName() << std::endl;
		return 0;
	}


	std::cout << "kicking " << client->getNickname() << " from channel " << channel->getName() << std::endl;
	part(IRCmsg(client, user_id(client->getNickname(), client->getUsername()), "PART", msg.getParameters(), msg.getTrailing()));
	return 0;
}

int	Server::mode(const IRCmsg& msg)
{
	Channel* 						channel = getChannelByName(msg.getParameters()[0]);
	Client*							sender = msg.getClient();
	const std::vector<std::string>&	params = msg.getParameters();
	const std::string				validModes = "iotkl";

	if (channel == NULL)
	{
		sender->appendToSendBuffer(ERR_NOSUCHCHANNEL(msg.getParameters()[0]));
		return 0;
	}

	if (params.size() == 1)
	{
		sender->appendToSendBuffer(RPL_CHANNELMODEIS(channel->getName(), channel->getModes()));
		return 0;
	}

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
					Client* client = getClientByNickname(params[1]);

					if (client == NULL)
						return 0;

					if (modeSet)
						channel->addOperator(client);
					else
						channel->removeOperator(client);
				}
				else if (param[i] == 'l')
				{
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

// const std::map<char, bool>&	Server::parseModes(const IRCmsg& msg)
// {
// 	Client*							sender = msg.getClient();
// 	const std::vector<std::string>&	params = msg.getParameters();
// 	const std::string				validModes = "iotkl";
// 	modeMap							res;

// 	for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it)
// 	{
// 		const std::string&	param = *it;

// 		if (param.empty() || (param[0] != '+' && param[0] != '-'))
// 			continue;

// 		bool	modeSet = (param[0] == '+');

// 		for (size_t i = 1; i < param.length(); ++i)
// 		{
// 			if (std::find(validModes.begin(), validModes.end(), param[i]) == validModes.end())
// 			{
// 				sender->appendToSendBuffer(ERR_UNKNOWNMODE(param[i]));
// 				return ;
// 			}

// 			res[param[i]] = modeSet;
// 		}
// 	}
// }