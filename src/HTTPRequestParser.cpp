/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 19:11:04 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/14 17:47:06 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPRequestParser.hpp"
#include <iostream>

static uint32_t	eatupsspaces(const std::string &str, uint32_t i)
{
	while (std::isspace(str[i]))
		i++;
	return (i);
}

static bool	parsefirstline(const std::string &req, std::map<std::string, std::string> &vals, int &status, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	std::string	method = req.substr(0, req.find_first_of(" \t\v\r\n\f"));
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		status = 400;
		return false;
	}
	vals.insert(std::pair<std::string, std::string>("type", method));
	i += method.size();

	i = eatupsspaces(req, i);
	std::cout << vals["type"] << std::endl;

	return true;
}

static bool	parseheaders(const std::string &req, std::map<std::string, std::string> &vals, int &status, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	return true;
}

static bool	parsebody(const std::string &req, std::map<std::string, std::string> &vals, int &status, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	return true;
}

HTTPRequestParser::HTTPRequestParser(const std::string &req)
{
	uint32_t	i = 0;

	if (parsefirstline(req, _vals, _status, i))
	{
		if (parseheaders(req, _vals, _status, i))
		{
			parsebody(req, _vals, _status, i);
			_status = 200;
		}
	}
}

HTTPRequestParser::~HTTPRequestParser() {}
