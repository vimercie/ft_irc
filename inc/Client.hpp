/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:13:05 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/23 16:27:00 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# define BUFFER_SIZE 512

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <map>
# include <poll.h>
# include <sys/socket.h>
# include <cerrno>
# include <cstdio>

class Channel;
class IRCmsg;

class	Client
{
	typedef void (Client::*cmd)(const IRCmsg& msg);

	private:
		pollfd						*socket;

		std::vector<std::string>	recvBuffer;
		std::vector<std::string>	sendBuffer;
		std::string					tmpBuffer;

		std::string					nickname;
		std::string					username;
		std::string					hostname;
		std::string					realname;
		std::string					mode;
		bool						pass;
		bool						toDisconnect;

		std::vector<Channel*>		channels;

	public:
		Client(pollfd *socket);
		~Client();

		bool	operator==(const Client& other) const;

		// getters
		pollfd								getSocket() const;
		const std::vector<std::string>&		getRecvBuffer() const;
		const std::vector<std::string>&		getSendBuffer() const;
		const std::string&					getTmpBuffer() const;
		const std::string&					getNickname() const;
		const std::string&					getUsername() const;
		const std::string&					getHostname() const;
		const std::string&					getRealname() const;
		const std::string&					getMode() const;
		const std::vector<Channel*>&		getChannels(void);
		bool								isAuthenticated() const;
		bool								isToDisconnect() const;

		// setters
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setHostname(const std::string& hostname);
		void	setRealname(const std::string& realname);
		void	setMode(const std::string& mode);
		void	setPass(bool pass);
		void	setToDisconnect(bool toDisconnect);

		// methods
		int		readFromSocket();
		int		sendToSocket();

		int		appendToRecvBuffer(const std::string& data);
		int		appendToSendBuffer(const std::string& data);
		int		appendToTmpBuffer(const std::string& data);
		void	clearRecvBuffer();
		void	clearSendBuffer();
		void	clearTmpBuffer();

		void	addChannel(Channel* channel);
		void	removeChannel(Channel* channel);
};

#endif