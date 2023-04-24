/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:04:02 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/24 11:32:13 by ntamayo-         ###   ########.fr       */
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

///Declaring the implemented status codes///////////////////////////////////////
std::map<std::string, std::string>	HTTPResponse::fillstatusmessages()
{
	std::map<std::string, std::string>	msgs;

	msgs.insert(std::pair<std::string, std::string>("200", "OK"));
	msgs.insert(std::pair<std::string, std::string>("400", "Bad Request"));
	msgs.insert(std::pair<std::string, std::string>("403", "Forbidden"));
	msgs.insert(std::pair<std::string, std::string>("404", "Not Found"));
	return (msgs);
}

std::map<std::string, std::string>	HTTPResponse::_statusMessages = HTTPResponse::fillstatusmessages();
////////////////////////////////////////////////////////////////////////////////

// First the internal, inherited request parser is constructed to have direct access to the map.
// Then the status is converted to a string in order to use it as a key.
// The first line of the response is then built by concatenation.
HTTPResponse::HTTPResponse(const HTTPRequestParser &givenRequest) : HTTPRequestParser(givenRequest)
{
	this->_strStatus = tostr(this->_status);

	this->_response = this->_vals["version"];
	this->_response += " " + this->_strStatus + " " + this->_statusMessages[this->_strStatus] + "\r\n";
}

HTTPResponse::~HTTPResponse() {}

std::string	HTTPResponse::getresponse() const
{
	return (this->_response);
}
