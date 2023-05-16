/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 20:54:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/16 19:51:24 by apena-ba         ###   ########.fr       */
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
    std::cout << "SERVER CREATED" << std::endl;
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

// void Server::handleRequest(HTTPRequestParser &parser, int client_socket)
// {
//     HTTPResponse    response(parser, this->_config);
//     unsigned int size = response.getresponse().length();
//     unsigned int cont = 0;
//     int ret = 0;
    
//     while(cont < size){
//         ret = write(client_socket, response.getresponse().c_str() + cont, size - cont);
//         if (ret < 0)
//             break;
//         cont += ret;
//     }
//     close(client_socket);
// }

bool Server::handleRequest(HTTPRequestParser &parser, int client_socket)
{
    map2::iterator it = this->_fds.find(client_socket);
    if(it == this->_fds.end()){
        HTTPResponse resp(parser, this->_config);
        this->_fds[client_socket] = std::pair<std::string, size_t>(resp.getresponse(), 0);
    }
    int retwrite = write(client_socket, this->_fds[client_socket].first.c_str() + this->_fds[client_socket].second, this->_fds[client_socket].first.size() - this->_fds[client_socket].second);
    if(retwrite >= 0)
        this->_fds[client_socket].second += retwrite;
    else
        std::cout << "WRITE FAILED" << std::endl;
    if(this->_fds[client_socket].second >= this->_fds[client_socket].first.size()){
        std::cout << "map size = " << this->_fds.size() << std::endl;
        it = this->_fds.find(client_socket);
        this->_fds.erase(it);
        return true;
    }
    return false;
}
