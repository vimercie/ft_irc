/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:24:34 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/10 15:48:51 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

extern int    status;

void statusHandler(int sig)
{
    if (sig == SIGINT)
    {
        std::cout << "SIGINT received" << std::endl;
        status = 0;
    }
}

std::vector<std::string> splitString(const std::string& input, const std::string& separator)
{
    std::vector<std::string>	result;
    size_t						found = 0;
	size_t						start = 0;

    while ((found = input.find(separator, start)) != std::string::npos)
	{
        result.push_back(input.substr(start, found - start));
        start = found + separator.length();
    }
    result.push_back(input.substr(start));

    return result;
}

std::string::const_iterator	getNextWord(std::string::const_iterator& it, const std::string& message, std::string& buffer)
{
	// skip les espaces
	while (isspace(*it) && it != message.end())
		it++;

	if (it == message.end())
		return it;

    std::string::const_iterator start = it;

    while (!isspace(*it) && it != message.end())
        it++;

    buffer = std::string(start, it);

	return it;
}

void	sendMsg(int fd, const std::string& msg)
{
	ssize_t	bytes_sent;

	bytes_sent = send(fd, msg.c_str(), msg.length(), 0);

	if (bytes_sent == -1)
		std::cerr << "Erreur d'envoi du message" << std::endl;
}
