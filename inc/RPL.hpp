/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:39:25 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/24 18:12:09 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#define user_id(nickname, username) (nickname + "!" + username + "@localhost")

// RPL stands for Reply. It is used to send responses to the client.
// ERR stands for Error. It is used to send error messages to the client.


#define RPL_WELCOME(nickname)									(":localhost 001 " + std::string(nickname) + " :Welcome to ft_irc " + std::string(nickname) + "\r\n")
#define RPL_WHOISUSER(nickname, username, hostname, realname)	(":localhost 311 " + std::string(nickname) + " " + std::string(username) + " " + std::string(hostname) + " * :" + std::string(realname) + "\r\n")
#define RPL_ENDOFWHOIS(nickname)								(":localhost 318 " + std::string(nickname) + " :End of WHOIS list\r\n")
#define RPL_CHANNELMODEIS(nick, channel, mode)  				(":localhost 324 " + std::string(nick) + " " + std::string(channel) + " +" + std::string(mode) + "\r\n")
#define RPL_CHANNELCREATED(channel, date)						(":localhost 329 " + std::string(channel)	+ " " + std::string(date) + "\r\n")
#define RPL_NOTOPIC(channel)									(":localhost 331 " + std::string(channel)	+ " :No topic is set\r\n")
#define RPL_TOPIC(client, channel, topic)						(":localhost 332 " + std::string(client)	+ " " + std::string(channel) + " :" + std::string(topic) + "\r\n")
#define RPL_INVITING(nickname, channel)							(":localhost 341 " + std::string(nickname)	+ " " + std::string(channel) + "\r\n")
#define RPL_NAMREPLY(channel, names)							(":localhost 353 " + std::string(channel)	+ " = " + std::string(names) + "\r\n")
#define RPL_ENDOFNAMES(channel)									(":localhost 366 " + std::string(channel)	+ " :End of NAMES list\r\n")


#define	ERR_NOSUCHNICK(nickname)								(":localhost 401 " + std::string(nickname)	+ " :No such nick/channel\r\n")
#define	ERR_NOSUCHCHANNEL(channel)								(":localhost 403 " + std::string(channel)	+ " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(channel)							(":localhost 404 " + std::string(channel)	+ " :Cannot send to channel\r\n")
#define ERR_TOOMANYTARGETS(command)								(":localhost 407 " + std::string(command)	+ " :Too many recipients\r\n")
#define	ERR_NORECIPIENT(command)								(":localhost 411 " + std::string(command)	+ " :No recipient given\r\n")
#define ERR_NOTEXTTOSEND()										(":localhost 412 * :No text to send\r\n")
#define ERR_UNKNOWNCOMMAND(command)								(":localhost 421 " + std::string(command)	+ " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN()									(":localhost 431 * :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nickname)							(":localhost 432 " + std::string(nickname)	+ " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(nickname)								(":localhost 433 " + std::string(nickname)	+ " :Nickname is already in use\r\n")
#define ERR_NOTONCHANNEL(channel)								(":localhost 442 " + std::string(channel)	+ " :You're not on that channel\r\n")
#define	ERR_NEEDMOREPARAMS(command)								(":localhost 461 " + std::string(command)	+ " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED()									(":localhost 462 * :You may not reregister\r\n")
#define	ERR_PASSWDMISMATCH()									(":localhost 464 * :Password incorrect\r\n")
#define	ERR_CHANNELISFULL(channel)								(":localhost 471 " + std::string(channel)	+ " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(flag)									(":localhost 472 " + std::string(1, flag)	+ " :is unknown mode char to me\r\n")
#define	ERR_INVITEONLYCHAN(channel)								(":localhost 473 " + std::string(channel)	+ " :Cannot join channel (+i)\r\n")
#define	ERR_BADCHANNELKEY(channel)								(":localhost 475 " + std::string(channel)	+ " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(channel)							(":localhost 482 " + std::string(channel)	+ " :You're not channel operator\r\n")
#define ERR_UMODEUNKNOWNFLAG(flag)								(":localhost 501 " + std::string(flag)		+ " :Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH(nickname)							(":localhost 502 " + std::string(nickname)	+ " :Cant change mode for other users\r\n")


#define NOTICE(sender, target, message)							(":" + std::string(sender) + " NOTICE " + std::string(target) + " :" + std::string(message) + "\r\n")