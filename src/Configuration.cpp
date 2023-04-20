/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 22:10:19 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/20 17:14:33 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"

Configuration::Configuration()
{
    this->_ports.push_back(8080);
}

Configuration::Configuration(std::vector<int> &ports, std::string &host)
{
    this->_ports = ports;
    this->_host = host;
}

Configuration::~Configuration(){}

Configuration &Configuration::operator=(Configuration const &to_equal)
{
    this->_ports = to_equal._ports;
    return(*this);
}

std::string Configuration::getHost() const
{
    return(this->_host);
}

std::vector<int> Configuration::getPorts() const
{
    return(this->_ports);
}
