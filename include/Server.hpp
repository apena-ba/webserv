/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:05:52 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/16 16:26:57 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "macro.hpp"
#include "Configuration.hpp"
#include <vector>

class Server{
    private:
        int                         _serverFd;
        std::vector<pollfd>         _pollFds;
        std::vector<std::string>    _requests;
        Configuration               _config;
        struct sockaddr_in          _address;
        socklen_t                   _addressLen;
        const struct timeval        _timeOutRead;
        const struct timeval        _timeOutWrite;
    public:
        Server();
        Server(Configuration &config);
        ~Server();
        void run(void);
        void updateFds(std::vector<pollfd> general_fds, unsigned int *index);
        void createNewClient(void);
        void closeClient(unsigned int index);
        bool setSockTimeOut(int fd);
        pollfd getPollfdByIndex(int index);
        unsigned int getPollfdsSize(void) const;
        timeval initializeTimeOutRead(void);
        timeval initializeTimeOutWrite(void);

        class FailSocketDeclarationException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return ("Problem setting socket");
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
};

#endif