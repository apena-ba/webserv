/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 19:11:04 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/05/15 19:33:18 by ntamayo-         ###   ########.fr       */
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

static void	makeitlower(std::string &str)
{
	for (uint i = 0; i < str.size(); i++)
		str[i] = std::tolower(str[i]);
}

static void	handlehost(std::string &host, std::map<std::string, std::string> &vals)
{
	std::string	port;
	size_t		portPos = host.find(":");
	
	if (portPos != std::string::npos)
	{
		port = host.substr(portPos + 1, host.find("\r\n"));
		host.erase(portPos, port.size() + 1);
		vals.insert(std::pair<std::string, std::string>("port", port));
	}
}

static void	handlequery(std::string &path, std::map<std::string, std::string> &vals)
{
	std::string	query;
	size_t		qpos = path.find("?");

	if (qpos != std::string::npos)
	{
		query = path.substr(qpos + 1);
		path.erase(qpos);
		vals.insert(std::pair<std::string, std::string>("query_string", query));
	}
}

bool	HTTPRequestParser::parsefirstline(const std::string &req, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	std::string	method = req.substr(0, req.find_first_of(" \t\v\r\n\f"));
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		std::cerr << ">> Error: 503, service unavailable." << std::endl;
		this->_status = 503;
		return false;
	}
	this->_vals.insert(std::pair<std::string, std::string>("type", method));
	i += method.size();

	i = eatupsspaces(req, i);

	std::string	uri, host, abs_path;
	if (!req.compare(i, 7, "http://"))
	{
		uri = "http://";
		i += 7;
		host = req.substr(i, req.find("/", i) - i);
		i += host.size();
		handlehost(host, this->_vals);
		this->_vals.insert(std::pair<std::string, std::string>("host", host));
	}
	else if (req.compare(i, 1, "/"))
	{
		std::cerr << ">> Error: 400, bad absolute path." << std::endl;
		this->_status = 400;
		return false;
	}
	else
		host = "";
	abs_path = req.substr(i, req.find_first_of(" \t\v\r\n\f", i) - i);
	uri += host + abs_path;
	i += abs_path.size();
	handlequery(abs_path, this->_vals);
	this->_vals.insert(std::pair<std::string, std::string>("location", abs_path));
	this->_vals.insert(std::pair<std::string, std::string>("uri", uri));

	i = eatupsspaces(req, i);

	std::string	version;
	if (req.compare(i, 5, "HTTP/"))
	{
		std::cerr << ">> Error: 400, bad version specifyer." << std::endl;
		this->_status = 400;
		return false;
	}
	version = req.substr(i, req.find_first_of(" \t\v\r\n\f", i) - i);
	if (version.compare(0, 9, "HTTP/1.1"))
	{
		std::cerr << ">> Error: 505, unsupported version." << std::endl;
		this->_status = 505;
		return false;
	}
	this->_vals.insert(std::pair<std::string, std::string>("version", version));
	i += version.size();
	if (req.compare(i, 2, "\r\n"))
	{
		std::cerr << ">> Error: 400, bad end of line character pair in '" << version << "' line." << std::endl;
		this->_status = 400;
		return false;
	}
	i += 2;

	return true;
}

// For each of the "\r\n" terminated lines (the header itself must end with "\r\n\r\n"):
// - Get the key delimited by a ':'
// - Check if the key is in the allowed list
// - Parse characters until the beginning of the value is found
// - Get the value delimited by the endline "\r\n"
// - Attempt to insert the key value pair. Return with error if the field id already stored
// - Skip until the end of line and check if it has the "\r\n"
bool	HTTPRequestParser::parseheaders(const std::string &req, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	while (req[i] && req.compare(i, 2, "\r\n"))
	{
		std::string	key = req.substr(i, req.find(":", i) - i);
		makeitlower(key);
		i += key.size() + 1; // Skip the ':' with that + 1

		i = eatupsspaces(req, i);

		std::string	field = req.substr(i, req.find("\r\n", i) - i);
		if (key == "host")
			handlehost(field, _vals);
		if (!this->_vals.insert(std::pair<std::string, std::string>(key, field)).second)
		{
			std::cerr << ">> Error: 400, duplicate field (" << key << ")." << std::endl;
			this->_status = 400;
			//return false;
		}
		i = req.find("\r\n", i);
		if (req.compare(i, 2, "\r\n"))
		{
			std::cerr << ">> Error: 400, bad end of line character pair in '" << key << "' line." << std::endl;
			this->_status = 400;
			return false;
		}
		i += 2;
	}
	if (!req[i])
	{
		std::cerr << ">> Error: 400, bad end of headers pair." << std::endl;
		this->_status = 400;
		return false;
	}
	i += 2;
	return true;
}

bool	HTTPRequestParser::parsebody(const std::string &req, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	std::string	body = req.substr(i);
	this->_vals.insert(std::pair<std::string, std::string>("body", body));
	return true;
}

HTTPRequestParser::HTTPRequestParser(const std::string &req)
{
	uint32_t	i = 0;

	this->_status = 200;
	// Disclaimer: The following functions are really fucking obtuse and unreadable, but the seem to get the job done
	if (parsefirstline(req, i))
	{
		if (parseheaders(req, i))
		{
			parsebody(req, i);
		}
	}
}

HTTPRequestParser::~HTTPRequestParser() {}

std::string	HTTPRequestParser::get(const std::string &key) const
{
	if (this->_vals.find(key) == this->_vals.end())
	{
		std::cerr << ">> Error: key '" << key << "' couldn't be found." << std::endl;
		return ("");
	}
	return (this->_vals.at(key));
}

const std::map<std::string, std::string>	&HTTPRequestParser::getVals() const
{
	return (this->_vals);
}

int	HTTPRequestParser::status() const
{
	return (this->_status);
}
