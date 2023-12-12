/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:07:43 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/12 15:32:54 by vimercie         ###   ########lyon.fr   */
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
		std::map<std::string, cmd>	cmds;
	public:
		Channel(std::string name);
		~Channel();

		std::string				getName(void) const;
		std::string				getNamesList(void);
		std::vector<Client*>	getClients(void) const;

		void					addClient(Client* client);
		void					removeClient(Client* client);
};

#endif