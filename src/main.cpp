/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/02 19:02:25 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include "ConfigurationParser/utils/ParsingUtils.hpp"
#include "ConfigurationParser/ConfigurationParser.hpp"

int main(int argc, char **argv) {
    try {
        std::string request = "GET /index.php HTTP/1.1\n"
                              "Host: example.com\n"
                              "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0\n"
                              "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\n"
                              "Accept-Language: en-US,en;q=0.5\n"
                              "Accept-Encoding: gzip, deflate, br\n"
                              "Connection: keep-alive\n"
                              "Upgrade-Insecure-Requests: 1";

        exit (1);
        ConfigurationParser parser;
        std::vector<Configuration> configs = parser.parse("clean_conf");
        std::cout << Configuration::getExtension("hello.html") << std::endl;
        Cluster cluster(configs);
        cluster.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}
