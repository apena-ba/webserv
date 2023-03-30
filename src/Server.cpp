/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:33:56 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/28 22:04:34 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _timeOutRead(this->initializeTimeOutRead()), _timeOutWrite(this->initializeTimeOutWrite())
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "SERVER fd = " << this->_fd << std::endl;
    if (this->_fd == -1 || this->_fd == 0)
        throw (Server::FailSocketDeclarationException());
    this->setSockTimeOut(this->_fd);
    this->_addressLen = sizeof(this->_address);
    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons(PORT);
    std::memset(this->_address.sin_zero, 0, this->_addressLen);
    if(bind(this->_fd, (struct sockaddr *)&this->_address, sizeof(this->_address)) == -1)
        throw (Server::FailBindException());
    if(listen(this->_fd, MAXCLIENT) == -1)
        throw (Server::FailListenException());
    memset(this->pollfds, 0, sizeof (this->pollfds));
    this->clients.push(this->_fd, POLLIN);
}

Server::~Server(){}

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
    {
        return (false);
    }
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const void *) &this->_timeOutWrite, sizeof(this->_timeOutWrite)) < 0)
    {
        return (false);
    }
    return (true);
}

void Server::run(void){
    int ret_poll;

    while(1){
        ret_poll = poll(this->clients.pollData(), this->clients.size(), 100);
        // -1 Error
        if (ret_poll == -1)
            throw (Server::FailPollException());
        // 0 nothing to read - maybe something to send
        else if (ret_poll == 0)
        {
            // std::cout << "Waiting for connection" << std::endl;
            this->sendData();
        }
        // positive something to read
        else
        {
            if (this->clients.getPollFdByIndex(0)->revents & POLLIN)
                this->createNewClient();
            this->checkConnections();
            this->readData();
            this->sendData();
        }
    }
}

void Server::checkConnections(void){
    for (unsigned int index = 1; index < this->clients.size(); index++)
    {
        if (this->clients.getPollFdByIndex(index)->fd > 0 && (this->clients.getPollFdByIndex(index)->revents & POLLHUP || this->clients.getPollFdByIndex(index)->revents & POLLERR))
        {
            close(this->clients.getPollFdByIndex(index)->fd);
            this->clients.remove(index);
            std::cout << "Error ocurred on index " << index << std::endl;
            break ;
        }
    }
}

void Server::createNewClient(void){
    std::cout << std::endl << "NEW CLIENT" << std::endl;

    int new_client;
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    new_client = accept(this->clients.getPollFdByIndex(0)->fd, (struct sockaddr *)&client_address, &client_address_length);
    if (new_client > 0 && this->setSockTimeOut(new_client) == true && fcntl (new_client, F_SETFL, O_NONBLOCK) == 0)
    {
        this->clients.push(new_client, POLLIN | POLLERR | POLLHUP);
    }
    else
    {
        std::cout << "Accept failed" << std::endl;
    }
}

void Server::readData(void){
    char buff[BUFFER_SIZE + 1];
    ssize_t ret_read;
    // Loop and reads from sockets and store the content into each client content
    for (unsigned int index = 1; index < this->clients.size(); index++)
    {
        memset(buff, 0, BUFFER_SIZE + 1);
        if ((this->clients.getPollFdByIndex(index)->fd > 0) && (this->clients.getPollFdByIndex(index)->revents & POLLIN)) {
            ret_read = read(this->clients.getPollFdByIndex(index)->fd, buff, BUFFER_SIZE);
            this->clients.setEvent(index, POLLOUT);
            if (ret_read < BUFFER_SIZE) {
                if (ret_read > 0) {
                    this->clients.pushContentToIndex(index, buff);
                }
            } else {
                this->clients.pushContentToIndex(index, buff);
            }
        }
    }
}

void Server::sendData(void){
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    std::string helloRequest(hello);
    static int responses = 0;

    // Loop until some socket is opened and is able to take output
    for (unsigned int index = 1; index < this->clients.size(); index++)
    {
        if ((this->clients.getPollFdByIndex(index)->revents & POLLOUT) && (this->clients.getPollFdByIndex(index)->fd > 0) && !(this->clients.getPollFdByIndex(index)->revents & POLLIN))
        {
            std::cout << std::endl << "RESPONSE NUMBER = " << responses++ << std::endl << std::endl;
            std::cout << "Content was = " << *(this->clients.getContentByIndex(index)) << std::endl;
            std::cout << "Size ->" << this->clients.size() << std::endl;
            write(this->clients.getPollFdByIndex(index)->fd, hello, helloRequest.length());
            close(this->clients.getPollFdByIndex(index)->fd);
            this->clients.remove(index);
        }
    }
}
