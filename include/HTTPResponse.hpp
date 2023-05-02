/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:51:55 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/04/28 15:11:08 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE
#define HTTPRESPONSE

#include "HTTPRequestParser.hpp"
#include "Configuration.hpp"

class HTTPResponse : public HTTPRequestParser
{
	public:
		HTTPResponse(const HTTPRequestParser &givenRequest, const Configuration &conf);
		~HTTPResponse();

		std::string	getresponse() const;
		std::string	getbody() const;

	private:
		static std::map<std::string, std::string>	fillstatusmessages();
		static std::map<uint, std::string>			fillerrorpages();
		void										bodybuilder(const Configuration &conf);
		void										get_perform(const Configuration &conf);
		void										pos_perform(const Configuration &conf);
		void										del_perform(const Configuration &conf);

		std::string									_strStatus;
		std::string									_response;
		std::string									_body;
		static std::map<std::string, std::string>	_statusMessages;
		static std::map<uint, std::string>			_errorPages;
};

#endif
