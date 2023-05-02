/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 19:24:53 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/02 19:30:47 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "macro.hpp"
#include "Configuration.hpp"
#include "HTTPRequestParser.hpp"
#include <vector>

class Server{
    private:
        Configuration           & _config;
        struct timeval    _timeOutWrite;

    public:
        Server();
        Server(Configuration const &config);
        ~Server();
        Server(Configuration &config);
        std::string getHost(void) const;
        void handleRequest(HTTPRequestParser &parser, int client_socket);
};

timeval initializeTimeOutWrite(void);
timeval initializeTimeOutRead(void);

#endif