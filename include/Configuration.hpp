/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:22:01 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/19 22:23:02 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP
#include <vector>
#include <iostream>

class Configuration{
        private:
                std::vector<int> _ports;
        public:
                Configuration();
                Configuration(std::vector<int> ports);
                ~Configuration();
                std::vector<int> getPorts() const;

                Configuration &operator=(Configuration const &to_equal);
};

#endif