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

    static std::string getExtension(const std::string &path) {
        std::string fileExtension = path.substr(path.find_last_of('.'));
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

    unsigned int checkPath(std::string path) {
        std::vector<std::string> vector;
        std::vector<unsigned int> index_location_in_path;
        unsigned int max_length_index;

        if (path.empty()) {
            throw ConfigurationException("Path is empty");
        }
        if (path[0] != '/') {
            throw ConfigurationException("Path must start with a '/'");
        }
        if (path.back() == '/' && path.length() > 1) {
            path.erase(path.length() - 1, 1);
        }
        for (unsigned int i = 0; i < routes.size(); i++) {
            if (routes[i].location.length() > path.length()) { continue; }
            if (path.find(routes[i].location) == 0
                && (path[routes[i].location.length()] == '/'
                    || path.length() == routes[i].location.length())) {
                index_location_in_path.push_back(i);
            }
        }
        if (index_location_in_path.empty()) {
            throw ConfigurationException("Path not found");
        }
        max_length_index = index_location_in_path[0];
        for (unsigned int j = 0; j < index_location_in_path.size(); j++) {
            if (routes[index_location_in_path[j]].location.length() > routes[max_length_index].location.length()) {
                max_length_index = index_location_in_path[j];
            }
        }
        return max_length_index;
    }

    const std::vector<unsigned int> ports;

    const unsigned int maxClients;

    const std::string defaultErrorPage;

    const unsigned int clientBodyMaxSize;

    const std::vector<Route> routes;

    const std::string host;

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
