/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:07:43 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/19 19:03:16 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define CHANNEL_NAME_MAX_LENGTH 50
# define NICKNAME_MAX_LENGTH 9

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>

class Client;
class IRCmsg;
class Command;
class Server;

class Channel
{
	typedef	void (Channel::*cmd)(const IRCmsg& msg);

	private:
		Server*						server;

		std::string 				name;
		std::vector<Client*>		clients;
		std::vector<Client*>		operators;
		std::vector<Client*>		invited;
		std::map<std::string, cmd>	cmds;  
		std::map<char , bool>		modes; // i, t, k, o, l 
		std::string					key; // password
		unsigned int				limit;
		std::string					topic;

	public:
		Channel(const std::string& name);
		~Channel();

	// getters
		std::string				getName(void) const;
		std::string				getNamesList(void);
		std::vector<Client*>	getClients(void) const;
		std::vector<Client*>	getOperators(void) const;
		std::string				getTopic(void) const;
		std::string				getKey(void) const;
		bool					getMode(char mode) const;
		std::string				getModes(void) const;
		unsigned int			getLimit(void) const;

		bool					isOperator(Client* client) const;
		bool					isInvited(Client* client) const;

	// setters
		void					setName(const std::string& name);
		void					setTopic(const std::string& topic);
		void					setKey(const std::string& key);
		void					setMode(char mode, bool value);
		void					setLimit(unsigned int limit);
		void					invite(Client* client);
		void					uninvite(Client* client);
		void					addClient(Client* client);
		void					removeClient(Client* client);

		void					addOperator(Client* client);
		void					removeOperator(Client* client);

		void					sendToChannel(const std::string& msg);
};

#endif