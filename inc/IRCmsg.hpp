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

class IRCmsg
{
	private:
		std::string					prefix;
		std::string					command;
		std::vector<std::string>	parameters;
		std::string					trailing;

	public:
		IRCmsg(const std::string& message);
		~IRCmsg();

	// parsing
		void		fromString(const std::string& message);
		std::string	toString() const;

		std::string::const_iterator	parseParameters(std::string::const_iterator& it, const std::string& message);
		std::string::const_iterator	getNextWord(std::string::const_iterator& start, const std::string& message, std::string& buffer);

	// debug
		void	displayMessage();
};

#endif