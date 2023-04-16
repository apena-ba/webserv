/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:08:07 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/16 16:40:44 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

Cluster::Cluster(){}

Cluster::Cluster(std::vector<Configuration> configs)
{
    for(unsigned int i = 0; i < configs.size(); i++)
        this->_servers.push_back(Server(configs[i]));
}

Cluster::~Cluster(){}

// METHODS

void Cluster::updateServerFds(void)
{
    unsigned int index = 0;

    for (unsigned int i = 0; i < this->_servers.size(); i++)
        this->_servers[i].updateFds(this->_fdsPoll, &index);
}

void Cluster::remakeFds(void)
{
    this->_fdsPoll.clear();
    for (unsigned int i = 0; i < this->_servers.size(); i++)
    {
        for(unsigned int x = 0; x < this->_servers[i].getPollfdsSize() ; x++)
            this->_fdsPoll.push_back(this->_servers[i].getPollfdByIndex(x));
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
