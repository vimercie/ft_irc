/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:35:42 by vimercie          #+#    #+#             */
/*   Updated: 2023/09/25 14:47:54 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define BUFFER_SIZE 1024

# include <iostream>
# include <unistd.h>
# include <cstdlib>
# include <string>
# include <cstring>
# include <vector>
# include <stdexcept>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>

class Channel;
class User;

class Server
{
	private:
		int 					_portno;
		std::string				_password;
		sockaddr_in 			_addr;
		socklen_t 				_addrLen;

		std::vector<User>		_users;
		std::vector<Channel>	_channels;
	public:
		Server(const std::string& port, const std::string& password);
		~Server();

		void init();
		void run();
};

#endif