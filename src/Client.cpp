/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:12:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/21 12:18:47 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/IRCmsg.hpp"
#include "../inc/Utils.hpp"

Client::Client(pollfd	*socket) : socket(socket), pass(false), toDisconnect(false) {std::cout << "Client created (fd: " << socket->fd << ")" << std::endl;}

Client::~Client() {std::cout << "Client destroyed" << std::endl;}

bool	Client::operator==(const Client& other) const {return nickname == other.nickname;}

// getters
pollfd		Client::getSocket() const {return *socket;}
const std::vector<std::string>&	Client::getRecvBuffer() const {return recvBuffer;}
const std::vector<std::string>&	Client::getSendBuffer() const {return sendBuffer;}
const std::string&	Client::getTmpBuffer() const {return tmpBuffer;}
const std::string&	Client::getNickname() const {return nickname;}
const std::string&	Client::getUsername() const {return username;}
const std::string&	Client::getHostname() const {return hostname;}
const std::string&	Client::getRealname() const {return realname;}
const std::string&	Client::getMode() const {return mode;}
const std::vector<Channel*>&	Client::getChannels(void) {return channels;}
bool		Client::isAuthenticated() const {return pass;}
bool		Client::isToDisconnect() const {return toDisconnect;}

// setters
void	Client::setNickname(const std::string& nickname) {this->nickname = nickname;}
void	Client::setUsername(const std::string& username) {this->username = username;}
void	Client::setHostname(const std::string& hostname) {this->hostname = hostname;}
void	Client::setRealname(const std::string& realname) {this->realname = realname;}
void	Client::setMode(const std::string& mode) {this->mode = mode;}
void	Client::setPass(bool pass) {this->pass = pass;}
void	Client::setToDisconnect(bool toDisconnect) {this->toDisconnect = toDisconnect;}

// methods
int	Client::readFromSocket()
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes_read;

	memset(buffer, 0, BUFFER_SIZE + 1);
	recvBuffer.clear();

	bytes_read = recv(socket->fd, buffer, BUFFER_SIZE, 0);

	if (bytes_read <= 0)
	{
		setToDisconnect(true);
		return 1;
	}
	else if (bytes_read > 0)
	{
		std::vector<std::string>	msgs = splitString(buffer, "\r\n");

		if (msgs.empty())
			return 0;

		for (std::vector<std::string>::iterator it = msgs.begin(); it != msgs.end(); it++)
		{
			appendToTmpBuffer(*it);

			if (it->empty())
				continue;

			if (getTmpBuffer().find("\r\n") != std::string::npos)
			{
				appendToRecvBuffer(getTmpBuffer());
				clearTmpBuffer();
			}
		}
	}

	return 0;
}

int	Client::sendToSocket()
{
	if (sendBuffer.empty())
		return 0;

	for (std::vector<std::string>::iterator it = sendBuffer.begin(); it != sendBuffer.end(); it++)
	{
		if (send(socket->fd, it->c_str(), it->length(), 0) < 0)
			return -1;
	}

	sendBuffer.clear();

	return 0;
}

void	Client::appendToRecvBuffer(const std::string& msg) {recvBuffer.push_back(msg);}
void	Client::appendToSendBuffer(const std::string& msg) {sendBuffer.push_back(msg);}
void	Client::appendToTmpBuffer(const std::string& msg) {tmpBuffer += msg;}

void	Client::clearRecvBuffer() {recvBuffer.clear();}
void	Client::clearSendBuffer() {sendBuffer.clear();}
void	Client::clearTmpBuffer() {tmpBuffer.clear();}

void	Client::addChannel(Channel* channel) {channels.push_back(channel);}
void	Client::removeChannel(Channel* channel)
{
	std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);

	if (it != channels.end())
		channels.erase(it);
}
