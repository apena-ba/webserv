/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 20:54:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/19 22:22:14 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Utils

timeval initializeTimeOutRead()
{
    timeval temp;
    temp.tv_sec = TIMEOUT_READING_SEC;
    temp.tv_usec = TIMEOUT_READING_USEC;
    return temp;
}

timeval initializeTimeOutWrite()
{
    timeval temp;
    temp.tv_sec = TIMEOUT_WRITING_SEC;
    temp.tv_usec = TIMEOUT_WRITING_USEC;
    return temp;
}

// CONSTRUCTOR AND DESTRUCTOR

Server::Server(Configuration const &config) : _timeOutWrite(initializeTimeOutWrite())
{
    this->_config = config;
}

void Server::handleRequest(std::string request, int client_socket)
{
    std::string hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
    std::cout << "Content was:" << std::endl << this->_requests[i] << std::endl;
    write(client_socket, hello.c_str(), hello.length());
    close(client_socket);
}
