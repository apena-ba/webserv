/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/15 22:31:32 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <iostream>

#define DEFAULT_PORT 80
#define DEFAULT_MAX_CLIENTS 100
#define DEFAULT_ERROR_PAGE 404
#define DEFAULT_CLIENT_BODY_MAX_SIZE 1000000

class Configuration {
public:
    Configuration() : port(DEFAULT_PORT), maxClients(DEFAULT_MAX_CLIENTS),
                      defaultErrorPage(DEFAULT_ERROR_PAGE),
                      clientBodyMaxSize(DEFAULT_CLIENT_BODY_MAX_SIZE) {}


    Configuration(unsigned int port, unsigned int maxClients, unsigned int defaultErrorPage,
                  unsigned int clientBodyMaxSize) : port(port), maxClients(maxClients),
                                                    defaultErrorPage(defaultErrorPage),
                                                    clientBodyMaxSize(clientBodyMaxSize) {}

    Configuration(unsigned int port) : port(port), maxClients(DEFAULT_MAX_CLIENTS),
                                       defaultErrorPage(DEFAULT_ERROR_PAGE),
                                       clientBodyMaxSize(DEFAULT_CLIENT_BODY_MAX_SIZE) {}

    ~Configuration() {}

    unsigned int getMaxClients() const { return maxClients; }

    unsigned int getPort() const { return port; }

    Configuration operator=(const Configuration &rhs) {
        return *this;
    }

    const unsigned int port;

    const unsigned int maxClients;

    const unsigned int defaultErrorPage;

    const unsigned int clientBodyMaxSize;
};

#endif
