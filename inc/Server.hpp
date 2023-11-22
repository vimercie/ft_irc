/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vimercie <vimercie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:35:42 by vimercie          #+#    #+#             */
/*   Updated: 2023/11/22 17:22:40 by vimercie         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

class IRCMessage;

class Server
{
private:
    int 				sockfd;
    struct sockaddr_in	servaddr;
    int 				port;
    std::string 		password;
public:
    Server(int port, const std::string& password);
    ~Server();

    void	acceptConnections();
    void	communicate(int connfd);
};

#endif