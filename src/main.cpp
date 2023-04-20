/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/19 21:53:36 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

std::vector<Configuration> config_parser(int argc, char **argv)
{
    std::vector<Configuration> configs;
    std::vector<int> ports;

    ports.push_back(8080);
    ports.push_back(8081);

    Configuration con1(ports);
    configs.push_back(con1);
    return(configs);
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        std::cerr << "USAGE: ./webserv [configuration file]" << std::endl;
        return (1);
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
        return (1);
    }
    return(0);
}
