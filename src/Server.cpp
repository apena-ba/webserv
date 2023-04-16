/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:33:56 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/16 16:46:17 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _timeOutRead(this->initializeTimeOutRead()), _timeOutWrite(this->initializeTimeOutWrite()){}

Server::Server(Configuration &config) : _timeOutRead(this->initializeTimeOutRead()), _timeOutWrite(this->initializeTimeOutWrite())
{
    this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "SERVER fd = " << this->_serverFd << std::endl;
    if (this->_serverFd < 1)
        throw (Server::FailSocketDeclarationException());
    this->setSockTimeOut(this->_serverFd);
    this->_addressLen = sizeof(this->_address);
    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons(config.getPort());
    
    std::memset(this->_address.sin_zero, 0, this->_addressLen);
    if(bind(this->_serverFd, (struct sockaddr *)&this->_address, sizeof(this->_address)) == -1)
        throw (Server::FailBindException());
    if(listen(this->_serverFd, config.getMaxClients()) == -1)
        throw (Server::FailListenException());
    this->_config = config;

    pollfd server_pollfd;
    server_pollfd.fd = this->_serverFd;
    server_pollfd.events = POLLIN | POLLERR | POLLHUP;
    server_pollfd.revents = 0;
    this->_pollFds.clear();
    this->_pollFds.push_back(server_pollfd);
    this->_requests.push_back("");
}

Server::~Server(){}

unsigned int Server::getPollfdsSize(void) const
{
    return this->_pollFds.size();
}

pollfd Server::getPollfdByIndex(int index)
{
    return this->_pollFds[index];
}

timeval Server::initializeTimeOutRead()
{
    timeval temp;
    temp.tv_sec = TIMEOUT_READING_SEC;
    temp.tv_usec = TIMEOUT_READING_USEC;
    return temp;
}

timeval Server::initializeTimeOutWrite()
{
    timeval temp;
    temp.tv_sec = TIMEOUT_WRITING_SEC;
    temp.tv_usec = TIMEOUT_WRITING_USEC;
    return temp;
}

bool Server::setSockTimeOut(int fd)
{
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const void *) &this->_timeOutRead, sizeof(this->_timeOutRead)) < 0)
        return (false);
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const void *) &this->_timeOutWrite, sizeof(this->_timeOutWrite)) < 0)
        return (false);
    return (true);
}

void Server::closeClient(unsigned int index)
{
    close(this->_pollFds[index].fd);
    this->_pollFds.erase(this->_pollFds.begin() + index);
    this->_requests.erase(this->_requests.begin() + index);
}

void Server::createNewClient(void)
{
    std::cout << std::endl << "NEW CLIENT" << std::endl;

    int new_client;
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    new_client = accept(this->_pollFds[0].fd, (struct sockaddr *)&client_address, &client_address_length);
    if (new_client > 0 && this->setSockTimeOut(new_client) == true && fcntl(new_client, F_SETFL, O_NONBLOCK) == 0)
    {
        pollfd client_pollfd;
        client_pollfd.fd = new_client;
        client_pollfd.events = POLLIN | POLLERR | POLLHUP;
        client_pollfd.revents = 0;
        this->_pollFds.push_back(client_pollfd);
        this->_requests.push_back("");
    }
}

void Server::updateFds(std::vector<pollfd> general_fds, unsigned int *index)
{
    for(unsigned int i = 0; i < this->_pollFds.size(); i++)
    {
        this->_pollFds[i] = general_fds[*index];
        *index += 1;
    }
}

void Server::run(void)
{
    int ret_read;
    char buff[BUFFER_SIZE + 1];
    std::vector<int> badFdsIndexes;

    if(this->_pollFds[0].revents & POLLIN)
        this->createNewClient();
    memset(buff, 0, BUFFER_SIZE + 1);
    for(unsigned int i = 1; i < this->_pollFds.size(); i++)
    {
        if(this->_pollFds[i].fd > 0 && this->_pollFds[i].revents & POLLIN) // Check reading
        {
            ret_read = read(this->_pollFds[i].fd, buff, BUFFER_SIZE);
            this->_pollFds[i].events |= POLLOUT;
            if (ret_read < BUFFER_SIZE && ret_read > 0)
                this->_requests[i].append(buff);
            else if (ret_read > 0)
                this->_requests[i].append(buff);
        }
        else if(this->_pollFds[i].fd > 0 && this->_pollFds[i].revents & POLLOUT && !(this->_pollFds[i].revents & POLLIN)) // Check writing
        {
            std::string hello("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
            static int responses = 0;
            std::cout << "Content was = " << this->_requests[i] << std::endl;
            write(this->_pollFds[i].fd, hello.c_str(), hello.length());
            this->closeClient(i);
        }
        else if(this->_pollFds[i].fd > 0 && (this->_pollFds[i].revents & POLLHUP || this->_pollFds[i].revents & POLLERR)) // Check error
            this->closeClient(i);
    }
}
