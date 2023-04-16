/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/15 22:31:32 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

class Configuration{
    private:
        int _port;
        unsigned int _maxClients;
    public:
        Configuration();
        Configuration(unsigned int port);
        ~Configuration();
        int getPort() const;
        unsigned int getMaxClients() const;
};

#endif