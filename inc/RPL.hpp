/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:39:25 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/17 15:37:15 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include <string>

// RPL stands for Reply. It is used to send responses to the client.
#define RPL_TOPIC(		channel, topic)	("#" + std::string(channel) + " TOPIC " + std::string(topic) + "\r\n")
#define RPL_TOPIC_SET(	channel, topic)	("332 " + std::string(channel) + " :" + std::string(topic) + "\r\n")
#define RPL_NAMREPLY(	channel, names)	("353 " + std::string(channel) + " = " + std::string(names) + "\r\n")
#define RPL_WELCOME(	nickname)			("001 " + std::string(nickname) + " :Welcome to ft_irc " + std::string(nickname) + "\r\n")
//