/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:07:43 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/06 15:56:03 by mmajani          ###   ########lyon.fr   */
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

class Channel
{
	typedef	void (Channel::*cmd)(const IRCmsg& msg);

	private:
		std::string 				name;
		std::vector<Client>			clients;
		// Command 					command;
		std::map<std::string, cmd>	cmds;
	public:
		Channel(std::string name);
		~Channel();

		std::vector<Client>	getClients() const;

		void	addClient(const Client& client);
		void	removeClient(const std::string& nickname);

		void	ircCmd(const IRCmsg& msg);

		void	welcome(const IRCmsg& msg);
};

#endif