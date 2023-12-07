/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:13:05 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/06 16:22:05 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <map>

class Command;

class	Client
{
	private:
		int			sock;

		std::string	nickname;
		std::string	username;
		std::string	password;
		std::string realname;
		std::string	mode;

	public:
		Client(int socket);
		~Client();

		bool	operator==(const Client& other) const;

		// setters
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setPassword(const std::string& password);
		void	setRealname(const std::string& realname);

};

#endif