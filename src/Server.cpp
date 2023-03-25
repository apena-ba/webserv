/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:33:56 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/25 22:35:35 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "fd = " << this->_fd << std::endl;
    if (this->_fd == -1 || this->_fd == 0)
        throw (Server::FailSocketDeclarationException());

    /*int opt = 1;
    if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cout << "setsockopt failed" << std::endl;
        exit (1);
    }*/
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
}

Server::~Server(){}

void Server::run(void){
    std::string client_content[MAXCLIENT];
    char i;
    int ret_poll;
    int new_client;
    int ret_read;
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    std::string helloRequest(hello);


    this->pollfds[1].fd = 0;
    this->pollfds[1].events = POLLIN;
    while(1){
        ret_poll = poll(this->pollfds, MAXCLIENT + 1, 100);
        //-1 error
        if (ret_poll == -1)
        {
            //throw poll exception
            std::cout << "poll fail" << std::endl;
            exit (1);
        }
            //timeout
        else if (ret_poll == 0)
        {
            //timeout
            std::cout << "timeout" << std::endl;
            continue;
        }
        else
        {
            //
            if (this->pollfds[0].revents & POLLIN)
            {
                std::cout << "new client" << std::endl;
                struct sockaddr_in client_address;
                socklen_t client_address_length = sizeof(client_address);
                new_client = accept(this->pollfds[0].fd,
                                    (struct sockaddr *)&client_address,
                                    &client_address_length);
                for (int index = 1; index != MAXCLIENT; index++)
                {
                    if (this->pollfds[index].fd == 0 || this->pollfds[index].fd == -1)
                    {
                        this->pollfds[index].fd = new_client;
                        this->pollfds[index].events = POLLIN | POLLOUT;
                        std::cout << "Break loop" << std::endl;
                        std::cout << "index = " << index << std::endl;
                        break ;
                    }
                }
                std::cout << "Loop stops" << std::endl;
            }
            for (int index = 1; index != MAXCLIENT; index++) {
                if (this->pollfds[index].fd > 0) {
                    if (this->pollfds[index].revents & POLLIN) {
                        std::cout << "Reading starts" << std::endl;
                        ret_read = read(this->pollfds[index].fd, &i, BUFFER_SIZE);
                        std::cout << "Reading stops" << std::endl;
                        if (ret_read <= 0) {
                            //client disconnect or error occur, remove the client
                            std::cout << "Close fd" << std::endl;
                            close(this->pollfds[index].fd);
                            this->pollfds[index].fd = -1;
                        } else {
                            client_content[index].push_back(i);
                        }
                    }
                    if (this->pollfds[index].revents & POLLOUT) {
                        if (this->pollfds[index].fd != -1) {
                            std::cout << client_content[index];
                            client_content[index].clear();
                            std::cout << "Write message" << std::endl;
                            write(this->pollfds[index].fd, hello, helloRequest.length());
                            close(this->pollfds[index].fd);
                            this->pollfds[index].fd = 0;
                        }
                    }
                }
            }
            //std::cout << "Second loop stops" << std::endl;
        }
    }
}
