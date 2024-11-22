/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:46:03 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/22 19:55:41 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _server(NULL), _connFd(-1), _requestStr(""), _request(NULL), _response(NULL)
{
	cout << "Client default constructor" << endl;
}

Client::Client(Server* server, int connFd) : 
	_server(server), _connFd(connFd), _requestStr(""), _request(NULL), _response(NULL)
{
	cout << "Client constructor" << endl;
}

Client::Client(const Client& src) : 
	_server(src._server), _connFd(src._connFd), _requestStr(src._requestStr), _request(src._request), _response(src._response)
{
	cout << "Client copy constructor" << endl;
}

Client::~Client()
{
	if (_request)
		delete _request;
	if (_response)
		delete _response;
	cout << "Client destructor" << endl;
}

Client&	Client::operator=(const Client& rhs)
{
	cout << "Client assignment operator" << endl;
	_server = rhs._server;
	_connFd = rhs._connFd;
	_requestStr = rhs._requestStr;
	_request = rhs._request;
	_response = rhs._response;
	return *this;
}

ReqStatus	Client::RecvRequest(string request)
{
	_requestStr += request;
	if (!IsCompleteRequest())
		return REQ_INCOMPLETE;
	if (BodyTooLarge())
		return REQ_BODY_TOO_LARGE;
	return REQ_COMPLETE;
}

bool	Client::IsCompleteRequest()
{
	// for GET & DELETE requests, the end is marked with \r\n\r\n
	if (_requestStr.find("GET") == 0 || _requestStr.find("DELETE") == 0)
	{
		if (_requestStr.find("\r\n\r\n") != std::string::npos)
			return true;
	}
	// for POST requests, the body size is given in the header
	if (_requestStr.find("POST") == 0)
	{
		std::string header = _requestStr.substr(0, _requestStr.find("\r\n\r\n"));
		std::string body = _requestStr.substr(_requestStr.find("\r\n\r\n") + 4);
		// if the header contains Content-Length
		if (header.find("Content-Length") != std::string::npos)
		{
			std::string contentLength = header.substr(header.find("Content-Length") + 16);
			contentLength = contentLength.substr(0, contentLength.find("\r\n"));
			long contentLengthInt = strtol(contentLength.c_str(), NULL, 10);
			cout << "contentLengthInt: " << contentLengthInt << endl;
			cout << "body.size(): " << body.size() << endl;
			if (static_cast<size_t>(contentLengthInt) == body.size())
				return true;
		}
		// if the header contains Transfer-Encoding: chunked
		if (header.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			if (body.find("0\r\n\r\n") != std::string::npos)
				return true;
		}
	}
	return false;
}

bool	Client::BodyTooLarge()
{
	std::string header = _requestStr.substr(0, _requestStr.find("\r\n\r\n"));
	std::string body = _requestStr.substr(_requestStr.find("\r\n\r\n") + 4);
	if (body.size() > _server->GetMaxBodySize())
	{
		_statusCode = 413;
		return true;
	}
	return false;
}

void	Client::ParseRequest()
{
	_request = new Request();
	// parse each line of header
	std::string header = _requestStr.substr(0, _requestStr.find("\r\n\r\n"));
	std::string body = _requestStr.substr(_requestStr.find("\r\n\r\n") + 4);
	std::string line;
	size_t pos = 0;
	while ((pos = header.find("\r\n")) != std::string::npos)
	{
		line = header.substr(0, pos);
		header.erase(0, pos + 2);
		if (line.empty())
			break;
		// if the line is the request line
		if (_request->GetMethod().empty())
		{
			// GET /index.html HTTP/1.1
			std::string method = line.substr(0, line.find(" "));
			line.erase(0, line.find(" ") + 1);
			std::string uri = line.substr(0, line.find(" "));
			line.erase(0, line.find(" ") + 1);
			std::string version = line;
			_request->SetMethod(method);
			_request->SetUri(uri);
			_request->SetVersion(version);
		}
		// if the line is a header field
		else
		{
			std::string field = line.substr(0, line.find(":"));
			line.erase(0, line.find(":") + 2);
			std::string value = line;
			_request->SetHeaderField(field, value);
		}
	}
	if (_request->GetMethod() == "POST")
		_request->SetBody(body);
}

void	Client::SearchLocation()
{
	// if the request is a GET request
	if (_request->GetMethod() == "GET")
	{
		// if the request is for the root directory
		if (_request->GetUri() == "/")
		{
			_statusCode = 200;
			return;
		}
		// if the request is for a file
		if (_request->GetUri().find(".") != std::string::npos)
		{
			_statusCode = 200;
			return;
		}
	}
	// if the request is a POST request
	if (_request->GetMethod() == "POST")
	{
		_statusCode = 200;
		return;
	}
	// if the request is a DELETE request
	if (_request->GetMethod() == "DELETE")
	{
		_statusCode = 200;
		return;
	}
	// if the request is not a GET, POST, or DELETE request
	_statusCode = 405;
}

void	Client::PrepareResponse()
{
	_response = new Response();
	_response->SetStatusCode(_statusCode);
	_response->SetStatusMessage();
	_response->SetServerName(_server->GetServerName());
	_response->SetContentLen(_response->GetBody().size());
	_response->SetConnection("close");
	if (_request->GetMethod() == "GET")
		_response->SetBody("Hello, World!");
	if (_request->GetMethod() == "POST")
		_response->SetBody("POST request received");
	if (_request->GetMethod() == "DELETE")
		_response->SetBody("DELETE request received");
	_response->SetContentLen(_response->GetBody().size());
	_response->ResembleResponse();
}

int	Client::GetConnFd() const
{
	return _connFd;
}

string&	Client::GetRequestStr()
{
	return _requestStr;
}

string&	Client::GetResponse()
{
	return _response->GetResponse();
}
