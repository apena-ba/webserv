/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:02:05 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/04 17:43:08 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Server.hpp"
#include "Port.hpp"
#include "Configuration.hpp"
#include "macro.hpp"

class Cluster {
    private:
        std::vector<Port>       _ports;
        std::vector<pollfd>     _fdsPoll;
        std::vector<Server>     _allServers;
    public:
        Cluster();
        Cluster(std::vector<Configuration> &configs);
        Cluster(Cluster const &to_copy);
        ~Cluster();
        
        // Methods
        void run(void);
        void updatePortsFds(void);
        void remakeFds(void);

        // Exceptions
        class FailPollException : public std::exception
        {
            virtual const char* what() const throw()
            {
                return ("Poll failed");
            };
        };
};

#endif