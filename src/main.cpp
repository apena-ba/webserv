/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:49:10 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/21 21:34:15 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv){

    Server serv;
    int fd = open("test");
    do
        {
            if (!read(fd, i, 1))
            {
                //exception
            }
            client_content = strdup(i);
        }
        while(read(fd, i, 1)){
            saver = client_content;
            client_content = strjoin(client_content, i);
            delete saver;        
        }
    return (0);
    if(argc != 2){
        std::cerr << "Wrong number of arguments" << std::endl;
        std::cerr << "USAGE: ./webserv [configuration file]" << std::endl;
        std::exit (1);
    }

    try
    {
        Server  server;
    } catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }

    std::cout << "SERVER ON" << std::endl;
    return(0);
}
