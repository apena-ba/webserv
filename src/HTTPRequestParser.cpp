/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 19:11:04 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/18 16:04:34 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPRequestParser.hpp"
#include <fstream>
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
	std::string	method = req.substr(0, req.find_first_of(" \t\v\r\n\f"));
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		std::cerr << ">> Error: 400, bad method." << std::endl;
		_status = 400;
		return false;
	}
	_vals.insert(std::pair<std::string, std::string>("type", method));
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
	// Check the files accessibility to exit with 403 or 404:
	/* std::ifstream	tempfd(abs_path);
	 * if (!tempfd.good())
	 * {
	 *     _status = 404;
	 *     if (tempfd.fail())
	 *         _status = 403;
	 *     return false;
	 * }
	 * tempfd.close(); */
	_vals.insert(std::pair<std::string, std::string>("host", host));
	_vals.insert(std::pair<std::string, std::string>("path", abs_path));
	_vals.insert(std::pair<std::string, std::string>("uri", uri));
	i += abs_path.size();

	i = eatupsspaces(req, i);

	std::string	version;
	if (req.compare(i, 5, "HTTP/"))
	{
		std::cerr << ">> Error: 400, bad version specifyer." << std::endl;
		_status = 400;
		return false;
	}
	version = req.substr(i, req.find_first_of(" \t\v\r\n\f", i) - i);
	if (version.compare(5, 4, "1.0") && version.compare(5, 4, "1.1") && version.compare(5, 2, "2") && version.compare(5, 2, "3"))
	{
		std::cerr << ">> Error: 400, bad version number." << std::endl;
		_status = 400;
		return false;
	}
	_vals.insert(std::pair<std::string, std::string>("version", version));
	i += version.size();
	if (req.compare(i, 2, "\r\n"))
	{
		std::cerr << ">> Error: 400, bad end of line character pair." << std::endl;
		_status = 400;
		return false;
	}
	i += 2;

	return true;
}

bool	HTTPRequestParser::parseheaders(const std::string &req, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	while (req[i] && req.compare(i, 2, "\r\n"))
	{
		std::string	key = req.substr(i, req.find(":", i) - i);
		makeitlower(key);
		if (!iskeygoodboi(key, _requestHeaderFields))
		{
			std::cerr << ">> Error: 400, bad request header field (" << key << ")." << std::endl;
			_status = 400;
			return false;
		}
		i += key.size() + 1; // Skip the ':' with that + 1

		i = eatupsspaces(req, i);

		std::string	field = req.substr(i, req.find_first_of("\r\n", i) - i);
		_vals.insert(std::pair<std::string, std::string>(key, field));
		i += field.size();
		if (req.compare(i, 2, "\r\n"))
		{
			std::cerr << ">> Error: 400, bad end of line character pair." << std::endl;
			_status = 400;
			return false;
		}
		i += 2;
	}
	if (!req[i])
	{
		std::cerr << ">> Error: 400, bad end of headers pair." << std::endl;
		_status = 400;
		return false;
	}
	return true;
}

bool	HTTPRequestParser::parsebody(const std::string &req, uint32_t &i)
{
	// If any errors arise, status is set to indicate the corresponding HTTP status code and the constructor returns.
	return true;
}

std::vector<std::string>	HTTPRequestParser::fillrequestheaderfields()
{
	std::vector<std::string>	fields;

	fields.push_back("accept");
	fields.push_back("accept-charset");
	fields.push_back("accept-encoding");
	fields.push_back("accept-language");
	fields.push_back("authorization");
	fields.push_back("expect");
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
	fields.push_back("te");
	fields.push_back("user-agent");
	return (fields);
}

std::vector<std::string>	HTTPRequestParser::_requestHeaderFields = HTTPRequestParser::fillrequestheaderfields();

HTTPRequestParser::HTTPRequestParser(const std::string &req)
{
	uint32_t	i = 0;

	if (parsefirstline(req, i))
	{
		if (parseheaders(req, i))
		{
			if (parsebody(req, i))
				_status = 200;
		}
	}
	// Delete this before uploading:
	std::cout << "status: " << _status << std::endl;
	for (auto &pair: _vals) {
		std::cout << pair.first << ": " << pair.second << std::endl;
	}
}

HTTPRequestParser::~HTTPRequestParser() {}
