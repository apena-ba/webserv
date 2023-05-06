/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/06 17:47:59 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include "ConfigurationParser/utils/ParsingUtils.hpp"
#include "ConfigurationParser/ConfigurationParser.hpp"

void cleanExit(int signal){
    (void)signal;
    std::cout << "\rStopping webserv" << std::endl;
    std::exit(0);
}

int main(int argc, char **argv, char **env) {
    try {
        signal(SIGINT, cleanExit);
        ConfigurationParser parser;
        std::vector<Configuration> configs = parser.parse("clean_conf");
        Cluster cluster(configs);
        cluster.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}
