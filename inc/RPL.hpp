/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 14:39:25 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/17 15:26:15 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// RPL stands for Reply. It is used to send responses to the client.

#define RPL_TOPIC		(channel, topic)	("#"	+ channel	+ " TOPIC "					+ topic		+ "\r\n")
#define RPL_TOPIC_SET	(channel, topic)	("332 " + channel	+ " :"						+ topic		+ "\r\n")
#define RPL_NAMREPLY	(channel, names)	("353 " + channel	+ " = "						+ names		+ "\r\n")
#define RPL_WELCOME		(nickname)			("001 " + nickname	+ " :Welcome to ft_irc "	+ nickname	+ "\r\n")
//