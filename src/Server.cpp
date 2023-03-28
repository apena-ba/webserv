/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:33:56 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/28 19:34:35 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "SERVER fd = " << this->_fd << std::endl;
    if (this->_fd == -1 || this->_fd == 0)
        throw (Server::FailSocketDeclarationException());

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
    this->pollfds[0].fd = this->_fd;
    this->pollfds[0].events = POLLIN;
    for(int index = 1; index < MAXCLIENT; index++)
    {
        this->pollfds[index].fd = -1;
        this->pollfds[index].events = 0;
        this->pollfds[index].revents = 0;
    }
}

Server::~Server(){}

void Server::run(void){
    int ret_poll;

    while(1){
        ret_poll = poll(this->pollfds, MAXCLIENT + 1, 100);
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
            if (this->pollfds[0].revents & POLLIN)
                this->createNewClient();
            this->checkConnections();
            this->readData();
            this->sendData();
        }
    }
}

void Server::checkConnections(void){
    for (int index = 1; index < MAXCLIENT; index++)
    {
        if (this->pollfds[index].fd > 0 && (this->pollfds[index].revents & POLLHUP || this->pollfds[index].revents & POLLERR))
        {
            close(this->pollfds[index].fd);
            this->pollfds[index].fd = -1;
            this->pollfds[index].events = 0;
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
    new_client = accept(this->pollfds[0].fd, (struct sockaddr *)&client_address, &client_address_length);

    for (int index = 1; index < MAXCLIENT; index++)
    {
        if (this->pollfds[index].fd < 1)
        {
            this->pollfds[index].fd = new_client;
            this->pollfds[index].events = POLLIN | POLLERR | POLLHUP;
            this->pollfds[index].revents = 0;
            fcntl(this->pollfds[index].fd, F_SETFL, O_NONBLOCK);
            break ;
        }
    }
}

void Server::readData(void){
    char buff[BUFFER_SIZE + 1];
    int ret_read;

    memset(buff, 0, BUFFER_SIZE + 1);
    // Loop and reads from sockets and store the content into each client content
    for (int index = 1; index < MAXCLIENT; index++)
    {
        if ((this->pollfds[index].fd > 0) && (this->pollfds[index].revents & POLLIN))
        {
            ret_read = read(this->pollfds[index].fd, buff, BUFFER_SIZE);
            this->pollfds[index].events |= POLLOUT;
            if (ret_read < BUFFER_SIZE)
            {
                if(ret_read > 0)
                    this->client_content[index].append(buff);
            }
            else
                this->client_content[index].append(buff);
            memset(buff, 0, BUFFER_SIZE + 1);
        }
    }
}

void Server::sendData(void){
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    std::string helloRequest(hello);
    static int responses = 0;

    // Loop until some socket is opened and is able to take output
    for (int index = 1; index < MAXCLIENT; index++)
    {
        if ((this->pollfds[index].revents & POLLOUT) && (this->pollfds[index].fd > 0) && !(this->pollfds[index].revents & POLLIN))
        {
            std::cout << std::endl << "RESPONSE NUMBER = " << responses++ << std::endl << std::endl;
            std::cout << "Content was = " << this->client_content[index] << std::endl;
            this->client_content[index].clear();
            write(this->pollfds[index].fd, hello, helloRequest.length());
            close(this->pollfds[index].fd);
            this->pollfds[index].fd = -1;
            this->pollfds[index].events = 0;
            this->pollfds[index].revents = 0;
        }
    }
}
