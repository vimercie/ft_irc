/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:13:05 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/09 01:10:54 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <map>

class Command;
class IRCmsg;

class	Client
{
	typedef void (Client::*cmd)(const IRCmsg& msg);

	private:
		int			socket;

		std::string	nickname;
		std::string	username;
		std::string	hostname;
		std::string realname;
		std::string	password;
		std::string	mode;

	public:
		Client(int socket);
		~Client();

		bool	operator==(const Client& other) const;

		// getters
		int		getSocket() const;
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getHostname() const;
		std::string	getRealname() const;
		std::string	getPassword() const;
		std::string	getMode() const;

		// setters
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setHostname(const std::string& hostname);
		void	setRealname(const std::string& realname);
		void	setPassword(const std::string& password);

		// commands
		void	execCmd(const IRCmsg& msg);
		void	nick(const IRCmsg& msg);
		void	user(const IRCmsg& msg);

		std::map<std::string, cmd>	cmds;
};

#endif