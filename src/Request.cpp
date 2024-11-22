/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:41:05 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/22 17:11:48 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : _method(""), _uri(""), _version(""), _header(), _body("")
{
}

Request::~Request()
{
}

void	Request::SetMethod(string method)
{
	_method = method;
}

void	Request::SetUri(string uri)
{
	_uri = uri;
}

void	Request::SetVersion(string version)
{
	_version = version;
}

void	Request::SetHeaderField(string field, string value)
{
	_header._fields[field] = value;
}

void	Request::SetBody(string body)
{
	_body = body;
}

string	Request::GetMethod() const
{
	return _method;
}

string	Request::GetUri() const
{
	return _uri;
}

string	Request::GetVersion() const
{
	return _version;
}

string	Request::GetHeaderField(string field) const
{
	return _header._fields.at(field);
}

string*	Request::GetBody() const
{
	return const_cast<string*>(&_body);
}
