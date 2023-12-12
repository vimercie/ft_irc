/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:13:05 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/12 15:47:53 by vimercie         ###   ########lyon.fr   */
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
		std::string				password;
		std::string				mode;

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
		std::string				getPassword() const;
		std::string				getMode() const;
		std::vector<Channel*>	getChannels(void);

		// setters
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setHostname(const std::string& hostname);
		void	setRealname(const std::string& realname);
		void	setPassword(const std::string& password);
		void	setMode(const std::string& mode);

		// methods
		void	addChannel(Channel* channel);
		void	removeChannel(Channel* channel);
};

#endif