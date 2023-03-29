/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:31:53 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/28 21:42:13 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Clients.hpp"


class Server{
    private:
        pollfd pollfds[MAXCLIENT + 1];
        std::string client_content[MAXCLIENT + 1];
        int _fd;
        struct sockaddr_in _address;
        socklen_t _addressLen;
        Clients clients;
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
        bool setSockTimeOut(int fd);

    class FailSocketDeclarationException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Problem setting timeout socket");
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