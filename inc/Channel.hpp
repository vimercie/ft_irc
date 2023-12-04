/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:07:43 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/03 19:15:55 by vimercie         ###   ########lyon.fr   */
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

class Channel
{
	typedef	void (Channel::*cmd)(const IRCmsg& msg);

	private:
		std::string 				name;
		std::vector<Client>			clients;
		std::map<std::string, cmd>	cmds;
	public:
		Channel(std::string name);
		~Channel();

		void	addClient(const std::string& nickname);
		// void	removeClient(const std::string& nickname);

		void	ircCmd(const IRCmsg& msg);

		void	welcome(const IRCmsg& msg);
		void	nick(const IRCmsg& msg);
		void	user(const IRCmsg& msg);
		void	join(const IRCmsg& msg);
		void	kick(const IRCmsg& msg);
		void	invite(const IRCmsg& msg);
		void	topic(const IRCmsg& msg);
		void	mode(const IRCmsg& msg);
};

#endif