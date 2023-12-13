/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:41:56 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/13 04:47:25 by vimercie         ###   ########lyon.fr   */
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
