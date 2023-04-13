/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 19:11:04 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/13 20:05:05 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPRequestParser.hpp"

static bool	parsefirstline(const std::string &req, std::map<std::string, std::string> &vals, int &status, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
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
		if (parseheaders(req, _vals, _status, i))
			parsebody(req, _vals, _status, i);
}

HTTPRequestParser::~HTTPRequestParser() {}
