/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntamayo- <ntamayo-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:51:55 by ntamayo-          #+#    #+#             */
/*   Updated: 2023/05/09 19:09:06 by ntamayo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE
#define HTTPRESPONSE

#include "HTTPRequestParser.hpp"
#include "Configuration.hpp"
#include "Cgi.hpp"

class HTTPResponse : public HTTPRequestParser
{
	public:
		HTTPResponse(const HTTPRequestParser &givenRequest, const Configuration &conf);
		~HTTPResponse();

		std::string	getresponse() const;
		std::string	getbody() const;
		static void	fillerrorpages(const Configuration &conf);

	private:
		static std::map<uint, std::string>	fillstatusmessages();
		static std::string					fetchErrorPage(const std::string &file);
		void								bodybuilder(const Configuration &conf);
		void								patharchitect(const Configuration &conf);
		void								get_perform(const Configuration &conf);
		void								pos_perform(const Configuration &conf);
		void								del_perform(const Configuration &conf);

		std::string													_strStatus;
		std::string													_response;
		std::string													_postHeaders;
		std::string													_body;
		const HTTPRequestParser										&_request;
		static std::map<uint, std::string>							_statusMessages;
		static std::map<std::string, std::map<uint, std::string> >	_errorPages;
};

#endif
