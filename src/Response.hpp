/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:31:50 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/22 19:31:00 by yrigny           ###   ########.fr       */
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
		void	InitializeStatusCodes();
		void	SetStatusCode(int code);
		void	SetStatusMessage();
		void	SetServerName(const string& serverName);
		void	SetConnection(const string& connection);
		void	SetContentLen(size_t len);
		void	SetBody(const string& body);
		void	SetResponse(const string& response);
		void	ResembleResponse();

		string	GetStatusCode() const;
		string	GetBody() const;
		string&	GetResponse();

	private:
		string	_statusCode;
		string	_statusMessage;
		string	_serverName;
		string	_contentLen;
		string	_connection;
		string	_body;
		string	_response;
};

#endif
