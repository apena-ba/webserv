/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apena-ba <apena-ba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:31:06 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/20 18:13:02 by apena-ba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER
#define HTTPREQUESTPARSER

#include <map>
#include <vector>
#include <string>

class HTTPRequestParser
{
	public:
		HTTPRequestParser(const std::string &req);
		~HTTPRequestParser();

		std::string	get(const std::string &key);
		int			status() const;

	private:
		static std::vector<std::string>	fillrequestheaderfields();
		bool							parsefirstline(const std::string &req, uint32_t &i);
		bool							parseheaders(const std::string &req, uint32_t &i);
		bool							parsebody(const std::string &req, uint32_t &i);

		int									_status;
		std::map<std::string, std::string>	_vals;
		static std::vector<std::string>		_requestHeaderFields;
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
