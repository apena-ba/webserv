/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/27 18:36:23 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include "ConfigurationParser/ParsingUtils.hpp"
#include "ConfigurationParser/ConfigurationParser.hpp"

int main(int argc, char **argv) {
    try {
        ConfigurationParser parser;
        std::vector<Configuration> configs = parser.parse("clean_conf");
        for (unsigned int i = 0; i < configs.size(); i++) {
            for (unsigned int j = 0; j < configs[i].routes.size(); j++) {
                for (unsigned int k = 0; k < configs[i].routes[j].methods.size(); k++) {
                    std::cout << configs[i].routes[j].methods[k] << std::endl;
                }
            }
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    exit(1);
}
