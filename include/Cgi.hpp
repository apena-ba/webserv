/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efournou <efournou@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:16:45 by efournou          #+#    #+#             */
/*   Updated: 2023/05/16 20:16:45 by efournou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include "HTTPRequestParser.hpp"
#include "Configuration.hpp"

class Cgi{
private:
public:
    static std::string process(const HTTPRequestParser &request, const Configuration & config, std::string stdin_content);
};

#endif
