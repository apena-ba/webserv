/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:31:53 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/28 21:24:36 by efournou         ###   ########.fr       */
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
#include <stdlib.h>
#include <sys/types.h>

#include "macro.hpp"


class Server{
    private:
        int _fd;
        struct sockaddr_in _address;
        socklen_t _addressLen;
        struct pollfd pollfds[MAXCLIENT + 1];
        std::string client_content[MAXCLIENT];
        const struct timeval _timeOutRead;
        const struct timeval _timeOutWrite;
        static timeval initializeTimeOutRead();
        static timeval initializeTimeOutWrite();
    public:
        Server();
        ~Server();
        void run();
        void readData();
        void sendData();
        void createNewClient();
        void checkConnections();
        int setSockTimeOut(int fd);

    class FailSocketDeclarationException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Problem setting timeout socket");
        };
    };

    class FailSocketSetTimeoutException : public std::exception
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
    
    class FailPollException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Poll failed");
        };
    };
};

#endif