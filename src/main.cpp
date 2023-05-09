/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/08 18:45:31 by apena-ba         ###   ########.fr       */
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

void ft(){system("leaks -q webserv");}

int main(int argc, char **argv, char **env) {
    if (argc != 2) {
        std::cerr << "Bad number of arguments" << std::endl;
        return (1);
    }
    atexit(ft);

    try {
        signal(SIGINT, cleanExit);
        ConfigurationParser parser;
        VECTOR_CONFIG configs = parser.parse(argv[1]);
        Cluster cluster(configs);
        cluster.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}
