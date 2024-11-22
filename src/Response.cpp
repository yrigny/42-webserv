/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:19:08 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/22 19:53:52 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

map<string, string> Response::statusCodes;
void Response::InitializeStatusCodes() {
    statusCodes["200"] = "OK";
    statusCodes["204"] = "No Content";
    statusCodes["400"] = "Bad Request";
    statusCodes["404"] = "Not Found";
    statusCodes["405"] = "Method Not Allowed";
    statusCodes["413"] = "Payload Too Large";
    statusCodes["500"] = "Internal Server Error";
    statusCodes["501"] = "Not Implemented";
    statusCodes["505"] = "HTTP Version Not Supported";
}

Response::Response()
{
}

Response::~Response()
{
}

void	Response::SetStatusCode(int code)
{
	_statusCode = code;
}

void	Response::SetStatusMessage()
{
	_statusMessage = statusCodes[_statusCode];
}

void	Response::SetServerName(const string& serverName)
{
	_serverName = serverName;
}

void	Response::SetConnection(const string& connection)
{
	_connection = connection;
}

void	Response::SetContentLen(size_t len)
{
	std::ostringstream oss;
	oss << len;
	_contentLen = oss.str();
}

void	Response::SetBody(const string& body)
{
	_body = body;
}

void	Response::SetResponse(const string& response)
{
	_response = response;
}

void	Response::ResembleResponse()
{
	_response = "HTTP/1.1 " + _statusCode + " " + _statusMessage + "\r\n";
	_response += "Server: " + _serverName + "\r\n";
	_response += "Connection: " + _connection + "\r\n";
	_response += "Content-Length: " + _contentLen + "\r\n";
	_response += "\r\n";
	_response += _body;
	cout << "----[ response ]----" << endl;
	cout << _response << endl;
	cout << "--------------------" << endl;
}

string	Response::GetStatusCode() const
{
	return _statusCode;
}

string	Response::GetBody() const
{
	return _body;
}

string&	Response::GetResponse()
{
	return _response;
}

