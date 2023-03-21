/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 19:33:56 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/21 20:05:59 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    this->_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_fd == 0)
        throw (Server::FailSocketDeclarationException());    
}

Server::~Server(){}
