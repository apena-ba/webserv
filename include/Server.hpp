/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:31:53 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/21 20:06:01 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <iostream>

class Server{
    private:
        int _fd;

    public:
        Server();
        ~Server();
    
    class FailSocketDeclarationException : std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Socket declaration failed");
        };
    };
    
    class FailBindException : std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Bind failed");
        };
    };

    class FailListenException : std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Listen failed");
        };
    };

    class FailAcceptException : std::exception
    {
        virtual const char* what() const throw()
        {
            return ("Accept failed");
        };
    };
};

#endif