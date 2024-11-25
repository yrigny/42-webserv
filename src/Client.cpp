/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:46:03 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/25 18:56:32 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _server(NULL), _connFd(-1), _requestStr(""), _request(NULL), _statusCode(""), _path(""), _response(NULL)
{
}

Client::Client(Server* server, int connFd) : 
	_server(server), _connFd(connFd), _requestStr(""), _request(NULL), _statusCode(""), _path(""), _response(NULL)
{
}

Client::Client(const Client& src) : 
	_server(src._server), _connFd(src._connFd), _requestStr(src._requestStr), _request(src._request), _statusCode(src._statusCode), _path(src._path), _response(src._response)
{
}

Client::~Client()
{
	if (_request)
		delete _request;
	if (_response)
		delete _response;
}

Client&	Client::operator=(const Client& rhs)
{
	_server = rhs._server;
	_connFd = rhs._connFd;
	_requestStr = rhs._requestStr;
	_request = rhs._request;
	_statusCode = rhs._statusCode;
	_path = rhs._path;
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
		_statusCode = "413";
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
	if (_request->GetMethod() == "GET")
	{
		// if the request is for the root directory
		if (_request->GetUri() == "/")
			_path = _server->GetRoot() + _server->GetIndexes()[0];
		// if the request is for a file
		if (_request->GetUri().find(".") != std::string::npos)
			_path = "." + _request->GetUri();
	}
	else if (_request->GetMethod() == "POST")
		_path = _server->GetUploadPath();
	else if (_request->GetMethod() == "DELETE")
		_path = _server->GetRoot() + _request->GetUri();
	// if the request is not a GET, POST, or DELETE request
	else
	{
		_statusCode = "405";
		_path = _server->GetErrorPages()[405];
	}
	// if the requested resource is not accessible
	ifstream	file(_path.c_str());
	if (!file.is_open())
	{
		_statusCode = "404";
		_path = _server->GetErrorPages()[404];
		return;
	}
	file.close();
}

void	Client::PrepareResponse()
{
	_response = new Response();
	_response->SetStatusCode(_statusCode);
	_response->SetStatusMessage();
	_response->SetContentType(_path);
	_response->SetServerName(_server->GetServerName());
	_response->SetConnection(_request->GetHeaderField("Connection"));
	if (_request->GetMethod() == "GET" && _response->GetContentType() == "text/html")
	{
		std::ifstream file(_path.c_str());
		std::string line;
		while (getline(file, line))
			_response->SetBody(_response->GetBody() + line + "\n");
		file.close();
	}
	if (_request->GetMethod() == "GET" && _response->GetContentType() == "image/jpeg")
	{
		std::ifstream file(_path.c_str(), std::ios::binary);
		std::string line;
		while (getline(file, line))
			_response->SetBody(_response->GetBody() + line + "\n");
		file.close();
	}
	if (_request->GetMethod() == "POST")
		_response->SetBody("POST request received");
	if (_request->GetMethod() == "DELETE")
		_response->SetBody("DELETE request received");
	_response->SetContentLen(_response->GetBody().size());
	_response->ResembleResponse();
}

void	Client::Reset()
{
	_requestStr = "";
	if (_request)
		delete _request;
	_request = NULL;
	_statusCode = "";
	_path = "";
	if (_response)
		delete _response;
	_response = NULL;
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
