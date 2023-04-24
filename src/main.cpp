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
#include "ConfigurationParser/ParsingUtils.hpp"
#include "ConfigurationParser/ConfigurationParser.hpp"

std::vector<Configuration> config_parser(int argc, char **argv) {
    std::vector<Configuration> configs;
    Configuration con1(8082);
    Configuration con2(8083);
    configs.push_back(con1);
    configs.push_back(con2);
    return (configs);
}

/*
 * std::string extract_file = ParsingUtils::fileToString("test");
    ParsingUtils::removeAllSpace(extract_file);
    std::vector<std::string> file = ParsingUtils::split(extract_file, "\n");
    std::vector<std::string> file2 = ParsingUtils::removeEmptyLine(file);
    ConfigurationParser parser;
    try {
        ParsingUtils::checkLimiter(file2, '{', '}');
        ParsingUtils::checkLimiter(file2, '[', ']');
        std::vector<std::vector<std::string> > servers = parser._splitServers(file2);
        parser._splitAllArgs(servers);
    }
    catch (ConfigurationParser::BadFile &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    catch (ParsingUtils::ErrorParsing &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    exit(1);
 */
int main(int argc, char **argv) {
    try {
        ConfigurationParser parser;
        std::vector<Configuration> configs = parser.parse("test");

        for (unsigned long i = 0; i < configs.size(); ++i) {
            std::cout << "port: " << configs[i].port << std::endl;
            std::cout << "max_clients: " << configs[i].maxClients << std::endl;
            std::cout << "default_error_page: " << configs[i].defaultErrorPage << std::endl;
            std::cout << "client_body_max_size: " << configs[i].clientBodyMaxSize << std::endl;
            std::cout << "--------------" << std::endl;
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    exit(1);
    if (argc != 2) {
        std::cerr << "Wrong number of arguments" << std::endl;
        std::cerr << "USAGE: ./webserv [configuration file]" << std::endl;
        std::exit(1);
    }

    try {
        std::vector<Configuration> configs = config_parser(argc, argv);
        Cluster cluster(configs);
        std::cout << "SERVER ON" << std::endl;
        cluster.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    return (0);
}
