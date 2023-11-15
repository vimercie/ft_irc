/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:48:21 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/15 15:10:11 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>

class Channel;

class	User
{
	private:
		std::string				_name;
		std::vector<Channel>	_channels;
	public:
		User(const std::string& username);
		~User();

		std::string					getUsername() const;
		std::vector<std::string>	getChannels() const;

		void						addChannel(const std::string& channel);
		void 						removeChannel(const std::string& channel);
};

#endif