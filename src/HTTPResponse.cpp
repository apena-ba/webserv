/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:04:02 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/28 15:12:36 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPResponse.hpp"
#include <strstream>

// The number to string conversion is done automatically by the string stream.
static std::string	tostr(int num)
{
	std::strstream	convert;
	convert << num;

	return (convert.str());
}

///Declaring the implemented status codes and default error pages///////////////
std::map<std::string, std::string>	HTTPResponse::fillstatusmessages()
{
	std::map<std::string, std::string>	msgs;

	msgs.insert(std::pair<std::string, std::string>("200", "OK"));
	msgs.insert(std::pair<std::string, std::string>("400", "Bad Request"));
	msgs.insert(std::pair<std::string, std::string>("403", "Forbidden"));
	msgs.insert(std::pair<std::string, std::string>("404", "Not Found"));
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
	if (access(this->_vals["abs_path"].c_str(), F_OK))
	{
		this->_status = 404;
		this->_body = this->_errorPages[this->_status];
		return;
	}
	std::ifstream	file(this->_vals["abs_path"]);
	if (file.bad())
	{
		this->_status = 403;
		this->_body = this->_errorPages[this->_status];
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

void	HTTPResponse::bodybuilder(const Configuration &conf)
{
	if (this->_status == 503) // Create the 503 error page.
	{
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
// [3]Iterate the map, building the header lines as simple '<key>: <value>\r\n' strings.
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
	for (std::map<std::string, std::string>::iterator it = this->_vals.begin(); it != this->_vals.end(); ++it)
	{
		this->_response += it->first + ": " + it->second + "\r\n";
	}
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
