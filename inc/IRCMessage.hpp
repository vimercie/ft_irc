/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:15:45 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/22 17:26:05 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_MESSAGE_H
# define IRC_MESSAGE_H

# include <iostream>
# include <string>
# include <cstring>
# include <vector>

class IRCMessage
{
	private:
		std::string					prefix;
		std::string					command;
		std::vector<std::string>	parameters;
		std::string					trailing;

	public:
		IRCMessage(const std::string& message);
		~IRCMessage();

		void parseMessage(const std::string& message);
		void displayParts() const;
};

#endif