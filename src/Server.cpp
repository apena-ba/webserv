/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:33:56 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/21 21:51:43 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "fd = " << this->_fd << std::endl;
    if (this->_fd == -1)
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
}

Server::~Server(){}

void Server::run(void){
    int client_socket_fd;
    char * client_content;
    char * saver;
    char *i;
    
    while(1){
        std::cout << "* WAITING FOR NEW CONNECTION *" << std::endl;
        write(1, "one\n", 4);
        if((client_socket_fd = accept(this->_fd, (struct sockaddr *)&this->_address, &this->_addressLen)) == -1)
            throw (Server::FailAcceptException());
        write(1, "two\n", 4);
    }
}
