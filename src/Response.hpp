/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:31:50 by yrigny            #+#    #+#             */
/*   Updated: 2024/12/10 16:24:36 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include <string>
# include <map>

using namespace std;

class Response
{
	public:
		Response();
		~Response();

		static	map<string, string> statusCodes;
		static	map<string, string> mimeTypes;
		void	InitializeStatusCodes();
		void	InitializeMimeTypes();
		void	SetStatusCode(const string& code);
		void	SetStatusMessage();
		void	SetContentType(const string& contentType);
		void	SetServerName(const string& serverName);
		void	SetConnection(const string& connection);
		void	SetContentLen(size_t len);
		void	SetBody(const string& body);
		void	SetResponse(const string& response);
		void	ResembleResponse();

		string	GetStatusCode() const;
		string	GetContentType() const;
		string	GetBody() const;
		string&	GetResponse();

	private:
		string	_statusCode;
		string	_statusMessage;
		string	_contentType;
		string	_serverName;
		string	_contentLen;
		string	_connection;
		string	_body;
		string	_response;
};

#endif
