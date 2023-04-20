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
}

Configuration::Configuration(std::vector<int> &ports)
{
    this->_ports = ports;
}

Configuration::~Configuration(){}

Configuration &operator=(Configuration const &to_equal)
{
    this->_ports = to_equal.ports();
}

std::vector<int> Configuration::getPorts() const
{
    return(this->_ports);
}
