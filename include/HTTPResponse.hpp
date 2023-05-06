/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:51:55 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/05/04 14:17:42 by ntamayo-         ###   ########.fr       */
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
		static std::map<uint, std::string>	fillstatusmessages();
		static std::map<uint, std::string>	fillerrorpages();
		static std::string					fetchErrorPage(const std::string &file);
		void								bodybuilder(const Configuration &conf);
		void								get_perform();
		void								pos_perform();
		void								del_perform();

		std::string							_strStatus;
		std::string							_response;
		std::string							_body;
		static std::map<uint, std::string>	_statusMessages;
		static std::map<uint, std::string>	_errorPages;
};

#endif
