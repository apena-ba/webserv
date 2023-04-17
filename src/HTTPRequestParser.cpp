/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 19:11:04 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/17 17:34:32 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPRequestParser.hpp"
#include <iostream>

static uint32_t	eatupsspaces(const std::string &str, uint32_t i)
{
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' || str[i] == '\f')
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

	std::string	uri, host, abs_path;
	if (!req.compare(i, 7, "http://"))
	{
		uri = "http://";
		i += 7;
	}
	host = req.substr(i, req.find("/", i) - i);
	i += host.size();
	abs_path = req.substr(i, req.find_first_of(" \t\v\r\n\f", i) - i);
	uri += host + abs_path;
	// Check the files accessibility to exit with 403 or 404 here?
	vals.insert(std::pair<std::string, std::string>("host", host));
	vals.insert(std::pair<std::string, std::string>("path", abs_path));
	vals.insert(std::pair<std::string, std::string>("uri", uri));
	i += abs_path.size();

	i = eatupsspaces(req, i);

	std::string	version;
	if (req.compare(i, 5, "HTTP/"))
	{
		status = 400;
		return false;
	}
	version = req.substr(i, req.find_first_of(" \t\v\r\n\f", i) - i);
	// Check version number
	vals.insert(std::pair<std::string, std::string>("version", version));
	i += version.size();
	if (req.compare(i, 2, "\r\n"))
	{
		status = 400;
		return false;
	}

	for (auto &pair: vals) {
		std::cout << pair.first << ": " << pair.second << std::endl;
	}
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
