/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCmsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:15:45 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/01 17:31:09 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_MESSAGE_H
# define IRC_MESSAGE_H

# include <iostream>
# include <string>
# include <cstring>
# include <vector>
# include <algorithm>

class Client;

class IRCmsg
{
	private:
		Client*						client;

		std::string					prefix;
		std::string					command;
		std::vector<std::string>	parameters;
		std::string					trailing;
	public:
		IRCmsg();
		IRCmsg(Client* client, const std::string& message);
		~IRCmsg();

	// getters
		Client*						getClient() const;
		std::string					getPrefix() const;
		std::string					getCommand() const;
		std::vector<std::string>	getParameters() const;
		std::string					getTrailing() const;
	// setters
		void						setPrefix(const std::string& prefix);
		void						setCommand(const std::string& command);
		void						setParameters(const std::vector<std::string>& parameters);
		void						setTrailing(const std::string& trailing);
		void						setClient(Client* client);

	// converters
		void						fromString(const std::string& message);
		std::string					toString() const;

	// parsing
		std::string::const_iterator	parseParameters(std::string::const_iterator& it, const std::string& message);

	// debug
		void	displayMessage() const;
};

#endif