/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:13:05 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/03 19:33:38 by vimercie         ###   ########lyon.fr   */
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
		std::string realname;
		std::string	mode;
	public:
		Client(const std::string& nickname);
		~Client();

		bool	operator==(const Client& other) const;

		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setPassword(const std::string& password);
		void	setRealname(const std::string& realname);
};

#endif