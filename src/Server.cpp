/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:33:56 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/21 22:01:13 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "fd = " << this->_fd << std::endl;
    if (this->_fd == -1 || this->_fd == 0)
        throw (Server::FailSocketDeclarationException());
    this->_addressLen = sizeof(this->_address);
    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons(PORT);
    std::memset(this->_address.sin_zero, 0, this->_addressLen);
    if(bind(this->_fd, (struct sockaddr *)&this->_address, sizeof(this->_address)) == -1)
        throw (Server::FailBindException());
    if(listen(this->_fd, 10) == -1)
        throw (Server::FailListenException());
    memset(this->pollfds, 0, sizeof (this->pollfds));
    this->pollfds[0].fd = this->_fd;
    this->pollfds[0].events = POLLIN;
}

Server::~Server(){}

void Server::run(void){
    std::string client_content;
    char i;
    int ret_poll;
    int new_client;
    int ret_read;

    while(1){
        ret_poll = poll(this->pollfds, 2, TIMEOUT_POLL);
        if (ret_poll == 0)
        {
            std::cout << "timeout" << std::endl;
            continue ;
        }
        if (ret_poll == -1)
        {
            //insted of exit and cout throw exception
            std::cout << "poll failed" << std::endl;
            exit (1);
        }
        //Mean server is in PollIn
        if (this->pollfds[0].revents & POLLIN)
        {
            if (this->pollfds[1].revents == 1 && client_content.empty() == false)
            {
                std::cout << "client content : " << std::endl;
                std::cout << client_content << std::endl;
                client_content.clear();
                continue;
            }
            new_client = accept(this->_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addressLen);
            if (new_client == -1)
            {
                //instead of exit and cout throw exception
                std::cout << "New client error" << std::endl;
                exit (1);
            }
            std::cout << "Creating a new client" << std::endl;
            this->pollfds[1].fd = new_client;
            this->pollfds[1].events = POLLIN;
        }
        //Mean client is in PollIn
        if (this->pollfds[1].revents & POLLIN)
        {
            ret_read = read(this->pollfds[1].fd, &i, 1);
            if (ret_read == -1) {
                //Dont exit and cout but throw exception
                std::cout << "Error reading from the client" << std::endl;
                exit(1);
            }
            else if (ret_read == 0)
            {
                std::cout << "Client disconnect" << std::endl;
                close (this->pollfds[1].fd);
                this->pollfds[1].fd = 0;
                continue;
            }
            else
            {
                std::cout << "appending" << std::endl;
                client_content.append(&i);
            }
        }
    }
}
