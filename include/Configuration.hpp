/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/20 17:14:06 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <vector>
#include <iostream>

class Configuration{
        private:
                std::vector<int>        _ports;
                std::string             _host;
        public:
                Configuration();
                Configuration(std::vector<int> &ports, std::string &host);
                ~Configuration();
                std::string getHost(void) const;
                std::vector<int> getPorts(void) const;

                Configuration &operator=(Configuration const &to_equal);
};

#endif