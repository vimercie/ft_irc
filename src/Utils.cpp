/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:24:34 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/08 18:43:20 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

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
