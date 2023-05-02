/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:04:02 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/05/02 18:27:17 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPResponse.hpp"

// The <<something>> to string conversion is done automatically by the string stream.
template<typename T> static std::string	tostr(T thang)
{
	std::string	ret;

	if (!thang)
		return ("0");
	while (thang)
	{
		std::string	tmp(1, thang % 10 + '0');
		thang /= 10;
		ret = tmp + ret;
	}
	return (ret);
}

///Declaring the implemented status codes and default error pages///////////////
std::map<std::string, std::string>	HTTPResponse::fillstatusmessages()
{
	std::map<std::string, std::string>	msgs;

	msgs.insert(std::pair<std::string, std::string>("200", "OK"));
	msgs.insert(std::pair<std::string, std::string>("400", "Bad Request"));
	msgs.insert(std::pair<std::string, std::string>("403", "Forbidden"));
	msgs.insert(std::pair<std::string, std::string>("404", "Not Found"));
	msgs.insert(std::pair<std::string, std::string>("405", "Method Not Allowed"));
	msgs.insert(std::pair<std::string, std::string>("413", "Payload Too Large"));
	msgs.insert(std::pair<std::string, std::string>("503", "Service Unavailable"));
	msgs.insert(std::pair<std::string, std::string>("505", "HTTP Version Not Supported"));
	return (msgs);
}

std::map<std::string, std::string>	HTTPResponse::_statusMessages = HTTPResponse::fillstatusmessages();

std::map<uint, std::string>	HTTPResponse::fillerrorpages()
{
	std::map<uint, std::string>	pags;

	pags.insert(std::pair<uint, std::string>(400, ""));
	pags.insert(std::pair<uint, std::string>(403, ""));
	pags.insert(std::pair<uint, std::string>(404, ""));
	pags.insert(std::pair<uint, std::string>(405, ""));
	pags.insert(std::pair<uint, std::string>(413, ""));
	pags.insert(std::pair<uint, std::string>(503, ""));
	pags.insert(std::pair<uint, std::string>(505, ""));
	return (pags);
}

std::map<uint, std::string>	HTTPResponse::_errorPages = HTTPResponse::fillerrorpages();
////////////////////////////////////////////////////////////////////////////////

void	HTTPResponse::get_perform(const Configuration &conf)
{
	// Check existance and try to open the requested file.
	if (access(this->_vals["location"].c_str(), F_OK))
	{
		this->_status = 404;
		this->_body = this->_errorPages[this->_status];
		return;
	}
	std::ifstream	file(this->_vals["location"]);
	if (file.bad())
	{
		this->_status = 403;
		this->_body = this->_errorPages[this->_status];
		file.close();
		return;
	}
	this->_body.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()); // Put the whole file into the body string.
	file.close();

	if (this->_body.size() > conf.clientBodyMaxSize)
	{
		this->_status = 413;
		this->_body = this->_errorPages[this->_status];
		return;
	}

	// Way of using the cgi to create the page?
}

void	HTTPResponse::pos_perform(const Configuration &conf) {}

void	HTTPResponse::del_perform(const Configuration &conf) {}

void	HTTPResponse::bodybuilder(const Configuration &conf)
{
	if (this->_status == 503) // Create the 503 error page.
	{
		this->_body = this->_errorPages[this->_status];
		return;
	}

	// Check if the given method is allowed for the given path:
	try
	{
		uint										pindex = conf.checkPath(this->_vals["location"]);
		std::vector<std::string>::const_iterator	it = conf.routes[pindex].methods.begin();

		for (; it != conf.routes[pindex].methods.end(); ++it)
			if (this->_vals["type"] == *it)
				break;
		if (it == conf.routes[pindex].methods.end())
		{
			this->_status = 405;
			this->_body = this->_errorPages[this->_status];
			return;
		}
	}
	catch (const std::exception &e)
	{
		this->_status = 404;
		this->_body = this->_errorPages[this->_status];
		return;
	}

	if (this->_vals["type"] == "GET")
		get_perform(conf);
	else if (this->_vals["type"] == "POST")
		pos_perform(conf);
	else if (this->_vals["type"] == "DELETE")
		del_perform(conf);
}

// First the internal, inherited request parser is constructed to have direct access to the map.
// The body is built following method specifications.
// [1]Then the status is converted to a string in order to use it as a key.
// [2]The first line of the response is then built by concatenation.
// [3]Build the header lines as simple '<key>: <value>\r\n' strings.
// [4]Add the body and voilà, a hot served response!
HTTPResponse::HTTPResponse(const HTTPRequestParser &givenRequest, const Configuration &conf) : HTTPRequestParser(givenRequest)
{
	// Build the body if the method requires it, try access again and update status accordingly
	bodybuilder(conf);

	// 1:
	this->_strStatus = tostr(this->_status);

	// 2:
	this->_response = this->_vals["version"] + " " + this->_strStatus + " " + this->_statusMessages[this->_strStatus] + "\r\n";

	// Write only the required headers, avoid repetition.
	// 3:
	this->_response += "Server: Jam⍺ Rushers' Webserv\r\n";
	/* if (this->_vals.find("connection") != this->_vals.end())
	 *     this->_response += "Connection: " + this->_vals["connection"] + "\r\n"; */
	this->_response += "Content-Length: " + tostr(this->_body.size()) + "\r\n\r\n";

	// 4:
	this->_response += this->_body;
}

HTTPResponse::~HTTPResponse() {}

std::string	HTTPResponse::getresponse() const
{
	return (this->_response);
}

std::string	HTTPResponse::getbody() const
{
	return (this->_body);
}
