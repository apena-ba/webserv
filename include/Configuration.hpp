/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/27 18:34:30 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <iostream>
#include "Route.hpp"

#define DEFAULT_MAX_CLIENTS 100
#define DEFAULT_ERROR_PAGE "error.html"
#define DEFAULT_CLIENT_BODY_MAX_SIZE 1000000

class Configuration {
private:

public:
    Configuration() : ports(), maxClients(DEFAULT_MAX_CLIENTS),
                      defaultErrorPage(DEFAULT_ERROR_PAGE),
                      clientBodyMaxSize(DEFAULT_CLIENT_BODY_MAX_SIZE) {};

    Configuration(unsigned int i);

    Configuration(const std::string &_host, unsigned int _maxClients, const std::string &_defaultErrorPage,
                  const std::vector<unsigned int> &_ports,
                  unsigned int _clientBodyMaxSize, const std::vector<Route> &_routes) : ports(_ports),
                                                                                        maxClients(_maxClients),
                                                                                        defaultErrorPage(
                                                                                                _defaultErrorPage),
                                                                                        clientBodyMaxSize(
                                                                                                _clientBodyMaxSize),
                                                                                        routes(_routes),
                                                                                        host(_host) {};

    ~Configuration() {}

    std::vector<int> getPorts() const {
        std::vector<int> vector;
        return vector;
    }

    std::string getHost() const {
        std::string host;
        return host;
    }

    unsigned int getMaxClients() const { return maxClients; }

    unsigned int getPort() const { return 1; }

    Configuration operator=(const Configuration &rhs) {
        return *this;
    }

    const std::vector<unsigned int> ports;

    const unsigned int maxClients;

    const std::string defaultErrorPage;

    const unsigned int clientBodyMaxSize;

    const std::vector<Route> routes;

    const std::string host;
};

#endif
