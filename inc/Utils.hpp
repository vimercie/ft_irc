/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmajani <mmajani@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:25:34 by vimercie          #+#    #+#             */
/*   Updated: 2023/12/09 15:42:04 by mmajani          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>
# include <csignal>
# include <iostream>

std::vector<std::string>	splitString(const std::string& input, const std::string& separator);
std::string::const_iterator	getNextWord(std::string::const_iterator& it, const std::string& message, std::string& buffer);

#endif