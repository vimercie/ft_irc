/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 00:07:43 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/22 17:29:34 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define CHANNEL_NAME_MAX_LENGTH 50

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>

class Channel
{
	typedef	void (Channel::*cmd)(void);

	private:
		std::string 				name;
		std::vector<std::string>	clients;
	public:
		Channel(std::string name);
		~Channel();
};

#endif