/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:51:55 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/26 13:42:57 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE
#define HTTPRESPONSE

#include <map>
#include <string>
#include "HTTPRequestParser.hpp"

class HTTPResponse : public HTTPRequestParser
{
	public:
		HTTPResponse(const HTTPRequestParser &givenRequest);
		~HTTPResponse();

		std::string	getresponse() const;
		std::string	getbody() const;

	private:
		static std::map<std::string, std::string>	fillstatusmessages();

		std::string									_strStatus;
		std::string									_response;
		std::string									_body;
		static std::map<std::string, std::string>	_statusMessages;
};

#endif
