/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:08:07 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/06 17:38:42 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include <algorithm>

// Cluster

// Cluster::Cluster() {}

Cluster::Cluster(std::vector<Configuration> &configs) {
    std::map<unsigned int, int> portsNumbers;

    // Get ports and number of ports
    for (unsigned int i = 0; i < configs.size(); i++) {
        std::vector<unsigned int> configPorts = configs[i].ports;
        for (unsigned int x = 0; x < configPorts.size(); x++) {
            std::map<unsigned int, int>::iterator it = portsNumbers.find(configPorts[x]);
            if (it != portsNumbers.end())
                portsNumbers[configPorts[x]]++;
            else
                portsNumbers[configPorts[x]] = 1;
        }
    }
    // Create servers
    for (unsigned int i = 0; i < configs.size(); i++)
        this->_allServers.push_back(Server(configs[i]));

    // Create ports
    for (std::map<unsigned int, int>::iterator it = portsNumbers.begin(); it != portsNumbers.end(); it++) {
        std::vector<Server *> portServers;

        for (unsigned int x = 0; x < configs.size(); x++) {
            std::vector<unsigned int> configPorts = configs[x].ports;
            std::vector<unsigned int>::iterator port = std::find(configPorts.begin(), configPorts.end(), it->first);
            if (port != configPorts.end())
                portServers.push_back(&this->_allServers[x]);
        }
        this->_ports.push_back(Port(it->first, portServers));
    }
}

Cluster::~Cluster() {}

// METHODS

void Cluster::updatePortsFds(void) {
    unsigned int index = 0;

    for (unsigned int i = 0; i < this->_ports.size(); i++) {
        this->_ports[i].updateFds(this->_fdsPoll, &index);
    }
}

void Cluster::remakeFds(void) {
    this->_fdsPoll.clear();
    for (unsigned int i = 0; i < this->_ports.size(); i++) {
        for (unsigned int x = 0; x < this->_ports[i].getPollfdsSize(); x++) {
            this->_fdsPoll.push_back(this->_ports[i].getPollfdByIndex(x));
        }
    }
}

void Cluster::run(void) {
    int ret_poll;

    while (1) {
        this->remakeFds();
        ret_poll = poll(this->_fdsPoll.data(), this->_fdsPoll.size(), 100);
        this->updatePortsFds();
        // -1 Error
        if (ret_poll == -1)
            throw (Cluster::FailPollException());
        // If the return is ok, we run every server
        for (unsigned int i = 0; i < this->_ports.size(); i++)
            this->_ports[i].run();
    }
}
