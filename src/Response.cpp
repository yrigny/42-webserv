/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 18:19:08 by yrigny            #+#    #+#             */
/*   Updated: 2024/12/10 16:24:57 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

map<string, string> Response::statusCodes;
void Response::InitializeStatusCodes()
{
    statusCodes["200"] = "OK";
    statusCodes["204"] = "No Content";
	statusCodes["301"] = "Moved Permanently";
    statusCodes["400"] = "Bad Request";
	statusCodes["403"] = "Forbidden";
    statusCodes["404"] = "Not Found";
    statusCodes["405"] = "Method Not Allowed";
    statusCodes["413"] = "Payload Too Large";
    statusCodes["500"] = "Internal Server Error";
    statusCodes["501"] = "Not Implemented";
    statusCodes["505"] = "HTTP Version Not Supported";
}

map<string, string> Response::mimeTypes;
void Response::InitializeMimeTypes()
{
	mimeTypes["html"] = "text/html";
	mimeTypes["htm"] = "text/html";
	mimeTypes["jpg"] = "image/jpeg";
	mimeTypes["jpeg"] = "image/jpeg";
	mimeTypes["png"] = "image/png";
	mimeTypes["gif"] = "image/gif";
	mimeTypes["css"] = "text/css";
	mimeTypes["js"] = "text/javascript";
	mimeTypes["json"] = "application/json";
	mimeTypes["xml"] = "application/xml";
	mimeTypes["ico"] = "image/x-icon";
	mimeTypes["svg"] = "image/svg+xml";
	mimeTypes["txt"] = "text/plain";
	mimeTypes["pdf"] = "application/pdf";
	mimeTypes["zip"] = "application/zip";
	mimeTypes["tar"] = "application/x-tar";
	mimeTypes["gz"] = "application/gzip";
	mimeTypes["mp3"] = "audio/mpeg";
	mimeTypes["mp4"] = "video/mp4";
}

Response::Response() :
	_statusCode(""),
	_statusMessage(""),
	_contentType(""),
	_serverName(""),
	_contentLen(""),
	_connection(""),
	_body(""),
	_response("")
{
	InitializeStatusCodes();
	InitializeMimeTypes();
}

Response::~Response()
{
}

void	Response::SetStatusCode(const string& code)
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

void	Response::SetContentType(const string& path)
{
	cout << "requested resource path: " << path << endl;
	size_t pos = path.find_last_of(".");
	string suffix = path.substr(pos + 1);
	_contentType = mimeTypes[suffix];
	// if doesn't exist in mimeTypes, set it to text/plain
	if (_contentType.empty())
		_contentType = "text/plain";
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
	_response += "Content-Type: " + _contentType + "\r\n";
	_response += "Server: " + _serverName + "\r\n";
	// _response += "Connection: " + _connection + "\r\n";
	_response += "Content-Length: " + _contentLen + "\r\n";
	_response += "\r\n";
	_response += _body;
	// cout << "----[ response ]----" << endl;
	// cout << _response << endl;
	// cout << "--------------------" << endl;
}

string	Response::GetStatusCode() const
{
	return _statusCode;
}

string	Response::GetContentType() const
{
	return _contentType;
}

string	Response::GetBody() const
{
	return _body;
}

string&	Response::GetResponse()
{
	return _response;
}

