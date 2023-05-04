/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 20:54:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/04 17:52:35 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "HTTPResponse.hpp"

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
    //std::string hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
    HTTPResponse    response(parser, this->_config);
    write(client_socket, response.getresponse().c_str(), response.getresponse().length());
    close(client_socket);
}
