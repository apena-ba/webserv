/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/16 20:16:49 by efournou         ###   ########.fr       */
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
                    const STRING &          _root,
                    const STRING &          _index,
                    const STRING &          _cgi,
                    const STRING &          _cgi_extension,
                    const VECTOR_ROUTE &    _routes) :
                    cgi_extension(_cgi_extension), cgi(_cgi),
                    root(_root),
                    index(_index),
                    ports(_ports),
                    maxClients(_maxClients),
                    defaultErrorPage(_defaultErrorPage),
                    clientBodyMaxSize(_clientBodyMaxSize),
                    routes(_routes), host(_host){}

    ~Configuration() {}
    // methods
    static  STRING      getExtension(const STRING &path);

    UINT                checkPath(STRING path) const;

    // fields configuration
    const STRING        cgi_extension;

    const STRING        cgi;

    const STRING        root;

    const STRING        index;

    const VECTOR_UINT   ports;

    const UINT          maxClients;

    const STRING        defaultErrorPage;

    const UINT          clientBodyMaxSize;

    const VECTOR_ROUTE  routes;

    const STRING        host;

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
typedef std::vector <Configuration> VECTOR_CONFIG;

#endif
