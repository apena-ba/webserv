/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/02 18:22:14 by ntamayo-         ###   ########.fr       */
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

    Configuration(UINT i);

    Configuration(const STRING &_host, UINT _maxClients, const STRING &_defaultErrorPage,
                  const VECTOR_UINT &_ports,
                  UINT _clientBodyMaxSize, const VECTOR_ROUTE &_routes) : ports(_ports),
                                                                                        maxClients(_maxClients),
                                                                                        defaultErrorPage(
                                                                                                _defaultErrorPage),
                                                                                        clientBodyMaxSize(
                                                                                                _clientBodyMaxSize),
                                                                                        routes(_routes),
                                                                                        host(_host) {};

    ~Configuration() {}

    VECTOR_INT getPorts() const {
        VECTOR_INT vector;
        return vector;
    }

    STRING getHost() const {
        STRING host;
        return host;
    }

    UINT getMaxClients() const { return maxClients; }

    UINT getPort() const { return 1; }

    Configuration operator=(const Configuration &rhs) {
        return *this;
    }

    static STRING getExtension(const STRING &path) {
        STRING fileExtension = path.substr(path.find_last_of('.'));
        if (fileExtension == ".py") {
            return ".py";
        }
        if (fileExtension == ".php") {
            return ".php";
        }
        if (fileExtension == ".html") {
            return ".html";
        }
        return "";
    }

    UINT checkPath(STRING path) const {
        VECTOR_STRING vector;
        VECTOR_UINT index_location_in_path;
        UINT max_length_index;

        if (path.empty()) {
            throw ConfigurationException("Path is empty");
        }
        if (path[0] != '/') {
            throw ConfigurationException("Path must start with a '/'");
        }
        if (path.back() == '/' && path.length() > 1) {
            path.erase(path.length() - 1, 1);
        }
        for (UINT i = 0; i < routes.size(); i++) {
            if (routes[i].location.length() > path.length()) { continue; }
            if (path.find(routes[i].location) == 0
                && (path[routes[i].location.length()] == '/'
                    || path.length() == routes[i].location.length())) {
                index_location_in_path.push_back(i);
            }
        }
        if (index_location_in_path.empty()) {
            for (UINT i = 0; i < routes.size(); i++) {
                if (routes[i].location == "/") {
                    return (i);
                }
            }
            throw ConfigurationException("Path not found");
        }
        max_length_index = index_location_in_path[0];
        for (UINT j = 0; j < index_location_in_path.size(); j++) {
            if (routes[index_location_in_path[j]].location.length() > routes[max_length_index].location.length()) {
                max_length_index = index_location_in_path[j];
            }
        }
        return max_length_index;
    }

    const VECTOR_UINT ports;

    const UINT maxClients;

    const STRING defaultErrorPage;

    const UINT clientBodyMaxSize;

    const VECTOR_ROUTE routes;

    const STRING host;

    class ConfigurationException : public std::exception {
    private:
        const char *_msg;
    public:
        ConfigurationException(const char *msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return (this->_msg);
        };
    };
};

#endif
