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
#include "macro.hpp"

class Configuration {
private:

public:
    Configuration(  const STRING &          _host,
                    UINT                    _maxClients,
                    const STRING &          _defaultErrorPage,
                    const VECTOR_UINT &     _ports,
                    UINT                    _clientBodyMaxSize,
                    const VECTOR_ROUTE &    _routes) : 
                    ports(_ports), maxClients(_maxClients),
                    defaultErrorPage(_defaultErrorPage),
                    clientBodyMaxSize(_clientBodyMaxSize),
                    routes(_routes),
                    host(_host) {}

    ~Configuration() {}

    static  STRING  getExtension(const STRING &path);

    UINT            checkPath(STRING path) const;
    
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
typedef std::vector <Configuration> VECTOR_CONFIGURATION;

#endif
