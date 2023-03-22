/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:31:53 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/21 21:46:42 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <iostream>
#include <list>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include "macro.hpp"

class Server{
    private:
        int _fd;
        struct sockaddr_in _address;
        socklen_t _addressLen;
        struct pollfd pollfds[2];

    public:
        Server();
        ~Server();
        void run();

    class FailSocketDeclarationException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Socket declaration failed");
        };
    };
    
    class FailBindException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Bind failed");
        };
    };

    class FailListenException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Listen failed");
        };
    };

    class FailAcceptException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Accept failed");
        };
    };
};

#endif