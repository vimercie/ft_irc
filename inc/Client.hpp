/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:13:05 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/13 04:32:54 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <map>
# include <poll.h>

class Channel;
class IRCmsg;

class	Client
{
	typedef void (Client::*cmd)(const IRCmsg& msg);

	private:
		pollfd					*socket;

		std::string				nickname;
		std::string				username;
		std::string				hostname;
		std::string				realname;
		std::string				mode;
		bool					pass;

		std::vector<Channel*>	channels;

	public:
		Client(pollfd *socket);
		~Client();

		bool	operator==(const Client& other) const;

		// getters
		pollfd					getSocket() const;
		std::string				getNickname() const;
		std::string				getUsername() const;
		std::string				getHostname() const;
		std::string				getRealname() const;
		std::string				getMode() const;
		std::vector<Channel*>	getChannels(void);
		bool					isAuthenticated() const;

		// setters
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setHostname(const std::string& hostname);
		void	setRealname(const std::string& realname);
		void	setMode(const std::string& mode);
		void	setPass(bool pass);

		// methods
		void	addChannel(Channel* channel);
		void	removeChannel(Channel* channel);
};

#endif