/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 20:54:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/17 10:36:21 by apena-ba         ###   ########.fr       */
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

Server::~Server(){}

Server::Server(Configuration &config) : _config(config), _timeOutWrite(initializeTimeOutWrite())
{
    std::cout << "SERVER CREATED" << std::endl;
	HTTPResponse::fillerrorpages(config);
}

std::string Server::getHost(void) const
{
    return (this->_config.host);
}

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
    if(this->_fds[client_socket].second >= this->_fds[client_socket].first.size()){
        it = this->_fds.find(client_socket);
        this->_fds.erase(it);
        return true;
    }
    return false;
}
