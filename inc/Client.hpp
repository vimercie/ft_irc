/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:48:21 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/22 16:54:13 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>

class Channel;

class	Client
{
	private:
		std::string	nickname;
		std::string	username;
		std::string	host;
	public:
		Client(const std::string& username);
		~Client();

		std::string					getUsername() const;
		std::vector<std::string>	getChannels() const;

		void						addChannel(const std::string& channel);
		void 						removeChannel(const std::string& channel);
};

#endif