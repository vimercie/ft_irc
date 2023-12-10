/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:39:17 by mmajani           #+#    #+#             */
/*   Updated: 2023/12/10 17:03:27 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# define USER 1
# define CHANNEL 2
# define SERVER 3

# include <iostream>
# include <string>
# include <vector>
# include <map>

class IRCmsg;
class Client;
class Channel;
class Server;

class Command
{
	private:
	// user commands
		static void	nick(const IRCmsg& msg);
		static void	user(const IRCmsg& msg);
	
	// utils
		static void welcome(Client* client);

	public:
		static void exec(const IRCmsg& msg);
};

#endif
