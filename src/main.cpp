/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/28 21:48:24 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv){

    if(argc != 2){
        std::cerr << "Wrong number of arguments" << std::endl;
        std::cerr << "USAGE: ./webserv [configuration file]" << std::endl;
        std::exit (1);
    }

    try
    {
        Server  server;
        std::cout << "SERVER ON" << std::endl;
        server.run();
    } catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
    return(0);
}
