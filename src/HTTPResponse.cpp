/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:04:02 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/05/06 15:56:09 by ntamayo-         ###   ########.fr       */
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
std::map<uint, std::string>	HTTPResponse::fillstatusmessages()
{
	std::map<uint, std::string>	msgs;

	msgs.insert(std::pair<uint, std::string>(200, "OK"));
	msgs.insert(std::pair<uint, std::string>(204, "No Content"));
	msgs.insert(std::pair<uint, std::string>(400, "Bad Request"));
	msgs.insert(std::pair<uint, std::string>(403, "Forbidden"));
	msgs.insert(std::pair<uint, std::string>(404, "Not Found"));
	msgs.insert(std::pair<uint, std::string>(405, "Method Not Allowed"));
	msgs.insert(std::pair<uint, std::string>(413, "Payload Too Large"));
	msgs.insert(std::pair<uint, std::string>(503, "Service Unavailable"));
	msgs.insert(std::pair<uint, std::string>(505, "HTTP Version Not Supported"));
	return (msgs);
}

std::map<uint, std::string>	HTTPResponse::_statusMessages = HTTPResponse::fillstatusmessages();

std::string	HTTPResponse::fetchErrorPage(const std::string &file)
{
	std::ifstream	in(file);
	std::string		ret;

	if (!in.is_open())
	{
		in.close();
		//in.open(conf.defaultErrorPage);
		if (!in.is_open())
			return ("SOME DEFAULT PAGE IN CASE THE DEFAULT PAGES FAIL LOL");
	}
	else
		ret.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
	return (ret);
}

std::map<uint, std::string>	HTTPResponse::fillerrorpages()
{
	std::map<uint, std::string>	pags;

	pags.insert(std::pair<uint, std::string>(204, fetchErrorPage("pages/204.html"))); // The body for this one shall be left empty.
	pags.insert(std::pair<uint, std::string>(400, fetchErrorPage("pages/400.html")));
	pags.insert(std::pair<uint, std::string>(403, fetchErrorPage("pages/403.html")));
	pags.insert(std::pair<uint, std::string>(404, fetchErrorPage("pages/404.html")));
	pags.insert(std::pair<uint, std::string>(405, fetchErrorPage("pages/405.html")));
	pags.insert(std::pair<uint, std::string>(413, fetchErrorPage("pages/413.html")));
	pags.insert(std::pair<uint, std::string>(503, fetchErrorPage("pages/503.html")));
	pags.insert(std::pair<uint, std::string>(505, fetchErrorPage("pages/505.html")));
	return (pags);
}

std::map<uint, std::string>	HTTPResponse::_errorPages = HTTPResponse::fillerrorpages();
////////////////////////////////////////////////////////////////////////////////

void	HTTPResponse::get_perform()
{
	// Check existance and try to open the requested file.
	if (access(this->_vals["location"].c_str(), F_OK))
	{
		this->_status = 404;
		this->_body = this->_errorPages[this->_status];
		return;
	}
	std::ifstream	file(this->_vals["location"]);
	if (!file.is_open())
	{
		this->_status = 403;
		this->_body = this->_errorPages[this->_status];
		file.close();
		return;
	}
	this->_body.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()); // Put the whole file into the body string.
	file.close();

	// Way of using the cgi to create the page?
}

void	HTTPResponse::pos_perform() {}

void	HTTPResponse::del_perform()
{
	// Check if file exists:
	if (access(this->_vals["location"].c_str(), F_OK))
	{
		this->_status = 404;
		this->_body = this->_errorPages[this->_status];
		return;
	}
	// Try to remove it. A non-zero return means remove failed:
	if (!std::remove(this->_vals["location"].c_str()))
		this->_status = 204; // The file was deleted. Nothing is returned.
	else
		this->_status = 403; // The file wasn't deleted, but it exists: Access forbidden.
	this->_body = this->_errorPages[this->_status];
}

void	HTTPResponse::patharchitect(const Configuration &conf)
{
	// The location field of the _vals map must be concatenated to the root given in the config.
	this->_vals["location"] = conf.root + this->_vals["location"];
}

void	HTTPResponse::bodybuilder(const Configuration &conf)
{
	if (this->_status == 503) // Create the 503 error page.
	{
		this->_body = this->_errorPages[this->_status];
		return;
	}

	patharchitect(conf);

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
		get_perform();
	else if (this->_vals["type"] == "POST")
		pos_perform();
	else if (this->_vals["type"] == "DELETE")
		del_perform();

	if (this->_body.size() > conf.clientBodyMaxSize)
	{
		this->_status = 413;
		this->_body = this->_errorPages[this->_status];
	}
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
	this->_response = this->_vals["version"] + " " + this->_strStatus + " " + this->_statusMessages[this->_status] + "\r\n";

	// Write only the required headers, avoid repetition.
	// 3:
	this->_response += "Server: Jam⍺ Rushers' Webserv\r\n";
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
