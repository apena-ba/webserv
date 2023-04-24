/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 19:11:04 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/24 19:31:18 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPRequestParser.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>

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

static bool	iskeygoodboi(const std::string &key, const std::vector<std::string> &requestHeaderFields)
{
	for (uint i = 0; i < requestHeaderFields.size(); i++)
		if (key == requestHeaderFields[i])
			return true;
	return false;
}

bool	HTTPRequestParser::parsefirstline(const std::string &req, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	std::cout << "REQUEST IS " << std::endl << req << std::endl;
	std::string	method = req.substr(0, req.find_first_of(" \t\v\r\n\f"));
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		std::cerr << ">> Error: 400, bad method." << std::endl;
		this->_status = 400;
		return false;
	}
	this->_vals.insert(std::pair<std::string, std::string>("type", method));
	i += method.size();

	i = eatupsspaces(req, i);

	std::string	uri, host, port, abs_path;
	if (!req.compare(i, 7, "http://"))
	{
		uri = "http://";
		i += 7;
		host = req.substr(i, req.find("/", i) - i);
		i += host.size();
		this->_vals.insert(std::pair<std::string, std::string>("host", host));
		size_t	portPos = host.find(":");
		if (portPos != std::string::npos)
		{
			port = host.substr(portPos);
			host.erase(portPos);
			this->_vals.insert(std::pair<std::string, std::string>("port", port));
		}
	}
	else
		host = "";
	abs_path = req.substr(i, req.find_first_of(" \t\v\r\n\f", i) - i);
	uri += host + abs_path;
	// Check the files accessibility to exit with 403 or 404:
	if (access(abs_path.c_str(), F_OK))
	{
		std::cerr << ">> Error: 404, file '" << abs_path << "' not found." << std::endl;
		this->_status = 404;
	}
	else if (access(abs_path.c_str(), R_OK))
	{
		std::cerr << ">> Error: 403, access to file '" << abs_path << "' if forbidden." << std::endl;
		this->_status = 403;
	}
	this->_vals.insert(std::pair<std::string, std::string>("path", abs_path));
	this->_vals.insert(std::pair<std::string, std::string>("uri", uri));
	i += abs_path.size();

	i = eatupsspaces(req, i);

	std::string	version;
	if (req.compare(i, 5, "HTTP/"))
	{
		std::cerr << ">> Error: 400, bad version specifyer." << std::endl;
		this->_status = 400;
		return false;
	}
	version = req.substr(i, req.find_first_of(" \t\v\r\n\f", i) - i);
	if (version.compare(5, 4, "1.0") && version.compare(5, 4, "1.1") && version.compare(5, 2, "2") && version.compare(5, 2, "3"))
	{
		std::cerr << ">> Error: 400, bad version number." << std::endl;
		this->_status = 400;
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
		if (!iskeygoodboi(key, this->_requestHeaderFields))
		{
			std::cerr << ">> Error: 400, bad request header field (" << key << ")." << std::endl;
			this->_status = 400;
			return false;
		}
		i += key.size() + 1; // Skip the ':' with that + 1

		i = eatupsspaces(req, i);

		std::string	field = req.substr(i, req.find_first_of("\r\n", i) - i);
		this->_vals.insert(std::pair<std::string, std::string>(key, field));
		/*if (!this->_vals.insert(std::pair<std::string, std::string>(key, field)).second)
		{
			std::cerr << ">> Error: 400, duplicate field (" << key << ")." << std::endl;
			this->_status = 400;
			return false;
		}*/
		i += field.size();
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

///Declaring the allowed header fields//////////////////////////////////////////
std::vector<std::string>	HTTPRequestParser::fillrequestheaderfields()
{
	std::vector<std::string>	fields;

	fields.push_back("accept");
	fields.push_back("accept-charset");
	fields.push_back("accept-encoding");
	fields.push_back("accept-language");
	fields.push_back("authorization");
	fields.push_back("expect");
	fields.push_back("connection");
	fields.push_back("from");
	fields.push_back("host");
	fields.push_back("if-match");
	fields.push_back("if-modified-since");
	fields.push_back("if-none-match");
	fields.push_back("if-range");
	fields.push_back("if-unmodified-since");
	fields.push_back("max-forwards");
	fields.push_back("proxy-authorization");
	fields.push_back("range");
	fields.push_back("referer");
	fields.push_back("sec-fetch-dest");
	fields.push_back("sec-fetch-mode");
	fields.push_back("sec-fetch-site");
	fields.push_back("sec-fetch-user");
	fields.push_back("te");
	fields.push_back("upgrade-insecure-requests");
	fields.push_back("user-agent");
	return (fields);
}

std::vector<std::string>	HTTPRequestParser::_requestHeaderFields = HTTPRequestParser::fillrequestheaderfields();
////////////////////////////////////////////////////////////////////////////////

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

std::string	HTTPRequestParser::get(const std::string &key)
{
	if (this->_vals.find(key) == this->_vals.end())
	{
		std::cerr << ">> Error: key '" << key << "' couldn't be found." << std::endl;
		return ("");
	}
	return (this->_vals[key]);
}

int	HTTPRequestParser::status() const
{
	return (this->_status);
}
