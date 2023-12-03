/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:13:05 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/02 21:41:30 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <map>

class	Client
{
	private:
		std::string	nickname;
		std::string	username;
		std::string	password;
	public:
		Client(const std::string& nickname);
		~Client();

		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setPassword(const std::string& password);
};

#endif