/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 19:24:53 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/19 22:21:02 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "macro.hpp"
#include "Configuration.hpp"
#include <vector>

class Server{
    private:
        Configuration           _config;
        const struct timeval    _timeOutWrite;

    public:
        Server();
        Server(Configuration &config);
        ~Server();
        void handleRequest(std::string &request, int client_socket);
};

timeval initializeTimeOutWrite(void);
timeval initializeTimeOutRead(void);

#endif