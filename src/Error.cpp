/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:41:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/13 15:50:19 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/IRCmsg.hpp"

std::string	Server::err_passwdmismatch()
{
	IRCmsg	response;

	response.setCommand("464");
	response.setPrefix("localhost");
	response.setParameters(std::vector<std::string>(1, "*"));
	response.setTrailing("Password incorrect");

	std::cout << "Sending: " << response.toString() << std::endl;

	return (response.toString());
}

std::string	Server::err_unknowncommand(const IRCmsg& msg)
{
	IRCmsg	response;
	std::vector<std::string> params;

	response.setCommand("421");
	response.setPrefix("localhost");

	params.push_back("*");
	params.push_back(msg.getCommand());
	response.setParameters(params);

	response.setTrailing("Unknown command");

	std::cout << "Sending: " << response.toString() << std::endl;

	return (response.toString());
}
