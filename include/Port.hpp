/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 19:16:15 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/20 20:30:24 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PORT_HPP
#define PORT_HPP

#include "Server.hpp"
#include "HTTPRequestParser.hpp"

class Port{
    private:
        unsigned int                _port;
        int                         _portFd;
        std::vector<pollfd>         _pollFds;
        std::vector<std::string>    _requests;
        std::vector<Server*>        _servers;
        struct sockaddr_in          _address;
        socklen_t                   _addressLen;
        const struct timeval        _timeOutRead;
        const struct timeval        _timeOutWrite;
    public:
        Port();
        Port(unsigned int portNumber, std::vector<Server*> servers);
        Port(Port const &to_copy);

        Port &operator=(Port const &to_equal);
        ~Port();

        // METHODS
        void run(void);
        void createNewClient(void);
        void closeClient(unsigned int index, bool closer);
        bool setSockTimeOut(int fd);
        void selectServer(unsigned int i);
        void updateFds(std::vector<pollfd> &general_fds, unsigned int *index);
        unsigned int getPollfdsSize(void) const;
        unsigned int getPort(void) const;
        pollfd getPollfdByIndex(int index);

        // EXCEPTIONS
        class FailSocketDeclarationException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return ("Problem setting socket");
            };
        };

        class FailTimeOutException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return ("Problem setting timeouts");
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