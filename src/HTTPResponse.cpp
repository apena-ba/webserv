/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:04:02 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/05/16 17:17:37 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HTTPResponse.hpp"

const std::string	ERROR500PAGE = "<!DOCTYPE html><html><header><title>500 Internal Server Error</title></header><body><center><h3>Error! 500</h3></center><center>Something went wrong. The server wasn't able to load the corresponding error page, so this is what you get :(</center></body></html>";
const std::string	ERROR301PAGE = "<!DOCTYPE html><html><header><title>301 Moved Permanently</title></header><body><center><h3>Error! 301</h3></center><center>It appears you've tried accessing forbidden realms... Begone you!</center></body></html>";

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
	msgs.insert(std::pair<uint, std::string>(301, "Moved Permanently"));
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
		return (ERROR500PAGE);
	else
		ret.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
	return (ret);
}

void	HTTPResponse::fillerrorpages(const Configuration &conf)
{
	_errorPages[conf.host].insert(std::pair<uint, std::string>(204, "")); // The body for this one shall be left empty.
	
	// This will only work properly if all paths in the config are stored with their absolute path using the config root.
	_errorPages[conf.host].insert(std::pair<uint, std::string>(301, ERROR301PAGE));
	_errorPages[conf.host].insert(std::pair<uint, std::string>(400, fetchErrorPage(conf.defaultErrorPage + "/400.html")));
	_errorPages[conf.host].insert(std::pair<uint, std::string>(403, fetchErrorPage(conf.defaultErrorPage + "/403.html")));
	_errorPages[conf.host].insert(std::pair<uint, std::string>(404, fetchErrorPage(conf.defaultErrorPage + "/404.html")));
	_errorPages[conf.host].insert(std::pair<uint, std::string>(405, fetchErrorPage(conf.defaultErrorPage + "/405.html")));
	_errorPages[conf.host].insert(std::pair<uint, std::string>(413, fetchErrorPage(conf.defaultErrorPage + "/413.html")));
	_errorPages[conf.host].insert(std::pair<uint, std::string>(503, fetchErrorPage(conf.defaultErrorPage + "/503.html")));
	_errorPages[conf.host].insert(std::pair<uint, std::string>(505, fetchErrorPage(conf.defaultErrorPage + "/505.html")));
}

std::map<std::string, std::map<uint, std::string> >	HTTPResponse::_errorPages;
////////////////////////////////////////////////////////////////////////////////

bool	HTTPResponse::read_file(const Configuration &conf)
{
	// Check existance and try to open the requested file.
	if (access(this->_vals["location"].c_str(), F_OK))
	{
		this->_status = 404;
		this->_body = this->_errorPages[conf.host][this->_status];
		return false;
	}
	std::ifstream	file(this->_vals["location"]);
	if (!file.is_open())
	{
		this->_status = 403;
		this->_body = this->_errorPages[conf.host][this->_status];
		file.close();
		return false;
	}
	this->_body.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()); // Put the whole file into the body string.
	file.close();
	return true;
}

std::string	HTTPResponse::run_cgi(const Configuration &conf)
{
	std::string	cgistr = Cgi::process(this->_request, conf, this->_body);

	if (cgistr.empty())
	{
		this->_status = 500;
		this->_body = ERROR500PAGE;
	}
	return cgistr;
}

void	HTTPResponse::store_cgi(const std::string &cgistr)
{
	size_t	nulain = cgistr.find("\r\n\r\n");
	this->_postHeaders = cgistr.substr(0, nulain + 2);
	this->_body = cgistr.substr(nulain + 4);
}

void	HTTPResponse::get_perform(const Configuration &conf)
{
	if (!read_file(conf))
		return;
	if (Configuration::getExtension(this->_vals["location"]) == conf.cgi_extension)
	{
		std::string	cgistr = run_cgi(conf);
		if (cgistr.empty())
			return;
		store_cgi(cgistr);
	}
}

void	HTTPResponse::pos_perform(const Configuration &conf)
{
	this->_body = this->_vals["body"];
	std::string	cgistr = run_cgi(conf);
	if (this->_isDir)
	{
		store_cgi(cgistr);
		return;
	}

	if (this->_vals["query_string"].find("create=true") == std::string::npos && access(this->_vals["location"].c_str(), F_OK))
	{
		this->_status = 404;
		this->_body = this->_errorPages[conf.host][this->_status];
		return;
	}
	std::ofstream	oFile(this->_vals["location"], std::ios::trunc); // Overwrite whatever was inside the file.
	if (!oFile.is_open())
	{
		this->_status = 404;
		this->_body = this->_errorPages[conf.host][this->_status];
		return;
	}
	oFile << this->_vals["body"];
	oFile.close();
	store_cgi(cgistr);
}

void	HTTPResponse::del_perform(const Configuration &conf)
{
	// Check if file exists:
	if (access(this->_vals["location"].c_str(), F_OK))
	{
		this->_status = 404;
		this->_body = this->_errorPages[conf.host][this->_status];
		return;
	}
	// Try to remove it. A non-zero return means remove failed:
	if (!std::remove(this->_vals["location"].c_str()))
		this->_status = 204; // The file was deleted. Nothing is returned.
	else
		this->_status = 403; // The file wasn't deleted, but it exists: Access forbidden.
	this->_body = this->_errorPages[conf.host][this->_status];
}

bool	HTTPResponse::isredir(uint pindex, const Configuration &conf)
{
	if (!conf.routes[pindex].redirection.empty())
	{
		this->_status = 301;
		this->_body = this->_errorPages[conf.host][this->_status];
		this->_postHeaders = "Location: " + conf.routes[pindex].redirection;
		return true;
	}
	return false;
}

void	HTTPResponse::patharchitect(const Configuration &conf)
{
	std::string	&loc = this->_vals["location"];

	if (this->_vals["location"].back() == '/')
		this->_isDir = true;

	// The location field of the _vals map must be concatenated to the root given in the config.
	loc = conf.root + loc;
}

void	HTTPResponse::bodybuilder(const Configuration &conf)
{
	if (this->_status != 200) // Create the 503 error page.
	{
		this->_body = this->_errorPages[conf.host][this->_status];
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
			{
				if (this->_vals["location"] == conf.routes[pindex].location)
					this->_vals["location"] = conf.routes[pindex].index;
				break;
			}
		if (it == conf.routes[pindex].methods.end())
		{
			this->_status = 405;
			this->_body = this->_errorPages[conf.host][this->_status];
			return;
		}
		if (isredir(pindex, conf))
			return;
	}
	catch (const std::exception &e)
	{
		this->_status = 404;
		this->_body = this->_errorPages[conf.host][this->_status];
		return;
	}

	if (this->_vals["type"] == "GET")
		get_perform(conf);
	else if (this->_vals["type"] == "POST")
		pos_perform(conf);
	else if (this->_vals["type"] == "DELETE")
		del_perform(conf);

	if (this->_body.size() > conf.clientBodyMaxSize)
	{
		this->_status = 413;
		this->_body = this->_errorPages[conf.host][this->_status];
	}
}

// First the internal, inherited request parser is constructed to have direct access to the map.
// The body is built following method specifications.
// [1]Then the status is converted to a string in order to use it as a key.
// [2]The first line of the response is then built by concatenation.
// [3]Build the header lines as simple '<key>: <value>\r\n' strings.
// [4]Add the body and voilà, a hot served response!
HTTPResponse::HTTPResponse(const HTTPRequestParser &givenRequest, const Configuration &conf) : HTTPRequestParser(givenRequest), _isDir(false), _postHeaders(""), _request(givenRequest)
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
	this->_response += "Content-Length: " + tostr(this->_body.size()) + "\r\n";
	this->_response += this->_postHeaders + "\r\n";

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
