/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 20:54:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/02 19:12:58 by apena-ba         ###   ########.fr       */
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

//Server::Server() : _config(Configuration()), _timeOutWrite(initializeTimeOutWrite()){}
Server::~Server(){}

Server::Server(Configuration &config) : _config(config), _timeOutWrite(initializeTimeOutWrite())
{
}
/*
Server & Server::operator= (const Server & ref )
{
    this->_config = ref._config;
    this->_timeOutWrite = ref._timeOutWrite;
    return *this;
}*/

std::string Server::getHost(void) const
{
    return (this->_config.host);
}

void Server::handleRequest(HTTPRequestParser &parser, int client_socket)
{
    std::string hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
    write(client_socket, hello.c_str(), hello.length());
    close(client_socket);
}
