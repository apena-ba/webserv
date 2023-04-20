/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:08:07 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/19 20:53:10 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

// Cluster

Cluster::Cluster(){}

Cluster::Cluster(std::vector<Configuration> configs)
{
    std::map<int, int> portsNumbers;

    // Get ports and number of ports
    for(unsigned int i = 0; i < configs.size(); i++)
    {
        for(unsigned int x = 0; x < configs[i].getPorts().size(); x++)
        {
            std::map<int, int>::iterator it = portsNumbers.find(configs[i].getPorts()[x]);
            if(it != portsNumbers.end())
                portsNumbers[configs[i].getPorts()[x]] = 1;
            else
                portsNumbers[configs[i].getPorts()[x]]++;
        }
    }

    // Create servers
    this->_allServers = new Server[configs.size()];
    for(unsigned int i = 0; i < configs.size(); i++)
        this->_allServers[i] = Server(configs[i]);

    // Create ports
    for(std::map<int, int>::iterator it = portsNumbers.begin(); it != portsNumbers.begin(); it++)
    {
        std::vector<Server*> portServers;
        
        for(unsigned int x = 0; x < configs.size(); x++)
        {
            std::vector<int> configPorts = configs[x].getPorts();
            std::vector<int>::iterator port = std::find(configPorts.begin(), configPorts.end(), it->first);
            if(port != configPorts.end())
                portServers.push_back(&this->_allServers[x]);
        }
        this->_ports.push_back(Port(it->first, portServers));
    }
}

Cluster::~Cluster(){}

// METHODS

void Cluster::updateServerFds(void)
{
    unsigned int index = 0;

    for (unsigned int i = 0; i < this->_ports.size(); i++)
        this->_ports[i].updateFds(this->_fdsPoll, &index);
}

void Cluster::remakeFds(void)
{
    this->_fdsPoll.clear();
    for (unsigned int i = 0; i < this->_ports.size(); i++)
    {
        for(unsigned int x = 0; x < this->_ports[i].getPollfdsSize() ; x++)
            this->_fdsPoll.push_back(this->_ports[i].getPollfdByIndex(x));
    }
}

void Cluster::run(void)
{
    int ret_poll;

    while(1)
    {
        this->remakeFds();
        ret_poll = poll(this->_fdsPoll.data(), this->_fdsPoll.size(), 100);
        this->updateServerFds();
        // -1 Error
        if (ret_poll == -1)
            throw (Cluster::FailPollException());
        // If the return is ok, we run every server
        for(unsigned int i = 0; i < this->_servers.size(); i++)
            this->_servers[i].run();
    }
}
