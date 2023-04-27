/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:51:55 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/27 12:08:39 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE
#define HTTPRESPONSE

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
		static std::map<uint, std::string>			fillerrorpages();
		void										bodybuilder();
		void										get_perform();
		void										pos_perform();
		void										del_perform();

		std::string									_strStatus;
		std::string									_response;
		std::string									_body;
		static std::map<std::string, std::string>	_statusMessages;
		static std::map<uint, std::string>			_errorPages;
};

#endif
