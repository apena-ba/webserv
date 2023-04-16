/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/16 16:43:53 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

std::vector<Configuration> config_parser(int argc, char **argv)
{
    std::vector<Configuration> configs;
    Configuration con1(8082);
    Configuration con2(8083);
    configs.push_back(con1);
    configs.push_back(con2);
    return(configs);
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        std::cerr << "USAGE: ./webserv [configuration file]" << std::endl;
        std::exit (1);
    }

    try
    {
        std::vector<Configuration> configs = config_parser(argc, argv);
        Cluster  cluster(configs);
        std::cout << "SERVER ON" << std::endl;
        cluster.run();
    } catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    return(0);
}
