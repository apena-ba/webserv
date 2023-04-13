/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:31:06 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/13 20:01:55 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER
#define HTTPREQUESTPARSER

#include <map>
#include <string>

class HTTPRequestParser
{
	public:
		HTTPRequestParser(const std::string &req);
		~HTTPRequestParser();

	private:
		int									_status;
		std::map<std::string, std::string>	_vals;
		/*
			The name of the keys for the start line elements are:
			type: (GET/POST/DELETE)
			uri:  the absoluteURI (beginning with: 'http://<<url>>'. The <<url>> part will be stored in the 'host' header variable) or absolute path (path to the solicited resource, beginning with a '/')
			version: the HTTP version used

			All other variables stored in the headers have their respective name as a key, in all lower case.

			body: a string containing all of the provided body part of the request.
		*/
};

#endif
