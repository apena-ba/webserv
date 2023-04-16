/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 22:10:19 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/16 01:08:02 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"

Configuration::Configuration()
{
    this->_port = 8080;
    this->_maxClients = 10;
}

Configuration::Configuration(unsigned int port)
{
    this->_port = port;
    this->_maxClients = 10;
}

Configuration::~Configuration(){}

int Configuration::getPort() const
{
    return(this->_port);
}

unsigned int Configuration::getMaxClients() const
{
    return(this->_maxClients);
}
