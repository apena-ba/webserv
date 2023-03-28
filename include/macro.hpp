/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:33:46 by apena-ba          #+#    #+#             */
/*   Updated: 2023/03/28 21:29:08 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACRO_HPP
#define MACRO_HPP

#include <sys/socket.h>
#include <iostream>
#include <list>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/types.h>

# define PORT 8082

# define TIMEOUT_POLL 100
# define TIMEOUT_READING_SEC 5
# define TIMEOUT_READING_USEC 0
# define TIMEOUT_WRITING_SEC 5
# define TIMEOUT_WRITING_USEC 0
# define MAXCLIENT 10
# define BUFFER_SIZE 1

#endif