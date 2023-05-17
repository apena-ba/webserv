/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:33:46 by apena-ba          #+#    #+#             */
/*   Updated: 2023/05/17 10:40:30 by apena-ba         ###   ########.fr       */
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
#include <string>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/types.h>

#define PORT 8085

#define TIMEOUT_POLL 10
#define TIMEOUT_READING_SEC 3
#define TIMEOUT_READING_USEC 1
#define TIMEOUT_WRITING_SEC 3
#define TIMEOUT_WRITING_USEC 1
#define MAXCLIENT 1000
#define BUFFER_SIZE 1000

// types
typedef std::string                 STRING;
typedef std::pair <STRING, STRING>  PAIR_STRING;
typedef std::vector <STRING>        VECTOR_STRING;
typedef unsigned int                UINT;
typedef std::vector <unsigned int>  VECTOR_UINT;
typedef std::vector <int>           VECTOR_INT;


#endif