/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:39:25 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/17 16:42:07 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#define user_id(nickname, username) (nickname + "!" + username + "@localhost")

// RPL stands for Reply. It is used to send responses to the client.


//invite
#define RPL_INVITING(nickname, channel)			("341 " + std::string(nickname)	+ " " + std::string(channel) + "\r\n")

//topic
#define RPL_NOTOPIC(channel)					("331" + std::string(channel)	+ " :No topic is set\r\n")
#define RPL_TOPIC(client, channel, topic)		("332 " + std::string(client)	+ " " + std::string(channel) + " :" + std::string(topic) + "\r\n")
#define RPL_SETTOPIC(channel, topic)			("TOPIC " + std::string(channel)	+ " :" + std::string(topic) + "\r\n")
//general
#define RPL_WELCOME(nickname)					("001 " + std::string(nickname) + " :Welcome to ft_irc " + std::string(nickname) + "\r\n")
#define RPL_NAMREPLY(channel, names)			("353 " + std::string(channel)	+ " = " + std::string(names) + "\r\n")
#define RPL_ENDOFNAMES(channel)					("366 " + std::string(channel)	+ " :End of NAMES list\r\n")

//channel modes
#define RPL_CHANNELMODEIS(channel, modes)		("324 " + std::string(channel)	+ " " + std::string(modes) + "\r\n")
#define RPL_CHANNELCREATED(channel, date)		("329 " + std::string(channel)	+ " " + std::string(date) + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(flag)				("501 " + std::string(flag)		+ " :Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH(nickname)			("502 " + std::string(nickname)	+ " :Cant change mode for other users\r\n")
