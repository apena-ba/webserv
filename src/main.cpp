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

int main(int argc, char **argv) {
    std::string extract_file = ParsingUtils::fileToString("test");
    ParsingUtils::removeAllSpace(extract_file);
    std::vector<std::string> file = ParsingUtils::split(extract_file, "\n");
    std::vector<std::string> file2 = ParsingUtils::removeEmptyLine(file);
    ConfigurationParser parser;
    try {
        ParsingUtils::checkLimiter(file2, '{', '}');
        ParsingUtils::checkLimiter(file2, '[', ']');
        std::vector<std::vector<std::string> > servers = parser.splitServers(file2);
        parser._splitAllArgs(servers);
        for (unsigned int i = 0; i < servers.size(); i++) {
            for (unsigned int j = 0; j < servers[i].size(); j++) {
                std::cout << servers[i][j] << std::endl;
            }
            std::cout << "----------------" << std::endl;
        }
        /*std::vector<std::vector<std::string> > file3 = parser.splitServers(file2);
        for (unsigned int i = 0; i < file3.size(); i++) {
            for (unsigned int j = 0; j < file3[i].size(); j++) {
                std::cout << file3[i][j] << std::endl;
            }
            std::cout << "----------------" << std::endl;
        }*/
        //parser._splitAllArgs(file3);
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
