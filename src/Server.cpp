/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 20:54:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/16 16:46:49 by apena-ba         ###   ########.fr       */
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
	HTTPResponse::fillerrorpages(config);
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
    HTTPResponse    response(parser, this->_config);
    unsigned int size = response.getresponse().length();
    unsigned int cont = 0;
    int ret = 0;
    
    while(cont < size){
        ret = write(client_socket, response.getresponse().c_str() + cont, size - cont);
        if (ret < 0)
            break;
        cont += ret;
    }
    close(client_socket);
}
