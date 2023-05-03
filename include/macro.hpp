/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:33:46 by apena-ba          #+#    #+#             */
/*   Updated: 2023/04/20 16:29:41 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACRO_HPP
#define MACRO_HPP

#include <sys/socket.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/types.h>
#include "Route.hpp"
#include "Configuration.hpp"

#define PORT 8085

#define TIMEOUT_POLL 100
#define TIMEOUT_READING_SEC 5
#define TIMEOUT_READING_USEC 0
#define TIMEOUT_WRITING_SEC 5
#define TIMEOUT_WRITING_USEC 0
#define MAXCLIENT 10
#define BUFFER_SIZE 1000

// types
typedef std::string                 STRING;
typedef std::pair <STRING, STRING>  PAIR_STRING;
typedef std::vector <STRING>        VECTOR_STRING;
typedef unsigned int                UINT;
typedef std::vector <unsigned int>  VECTOR_UINT;
typedef std::vector <int>           VECTOR_INT;
typedef std::vector <Route>         VECTOR_ROUTE;
typedef std::vector <Configuration> VECTOR_CONFIGURATION;

#endif