/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:07:43 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/15 16:41:33 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>

class User;

class Channel
{
	typedef	void (Channel::*cmd)(void);

	private:
		std::string 				_name;
		std::vector<User>			_users;
		std::map<std::string, cmd>	_cmds;
	public:
		Channel(std::string name);
		~Channel();

		std::string			getName() const;
		std::vector<User>	getUsers() const;

		void				addUser(const User& user);
		void 				removeUser(const User& user);

		std::string			parseName(const std::string& name);

		// Commandes
		void				kick();
		void				invite();
		void				topic();
		void				mode();
};

#endif