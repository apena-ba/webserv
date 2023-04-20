/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:02:05 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/19 21:54:31 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Server.hpp"
#include "Configuration.hpp"
#include "macro.hpp"

class Cluster {
    private:
        std::vector<Port>       _ports;
        std::vector<pollfd>     _fdsPoll;
        Server                  *_allServers;
    public:
        Cluster();
        Cluster(std::vector<Configuration> configs);
        Cluster(Cluster const &to_copy);
        ~Cluster();
        
        // Methods
        void run(void);
        void updateServerFds(void);
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