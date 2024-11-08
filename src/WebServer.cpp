/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:08:14 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/07 14:51:47 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer()
{
	// http_conn
	// root path
	// timer
}

WebServer::~WebServer()
{
	close(_listenFd);
}

void WebServer::InitServer()
{
	// create a socket descriptor
	_listenFd = socket(PF_INET, SOCK_STREAM, 0); // IPv4, TCP
	if (_listenFd == -1)
	{
		Log::LogMsg(ERROR, "socket() failed");
		exit(1);
	}

	// set the socket address
	struct sockaddr_in addr; // IPv4 address
	memset(&addr, sizeof(addr), 0); // zero out the structure
	addr.sin_family = AF_INET; // IPv4
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, host to network long
	addr.sin_port = htons(8080); // host to network short
	
	// Allow socket descriptor to be reuseable
	int flag = 1;
	if (setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
	{
		Log::LogMsg(ERROR, "setsockopt() failed");
		close(_listenFd);
		exit(1);
	}

	// bind the socket
	if (bind(_listenFd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		Log::LogMsg(ERROR, "bind() failed");
		close(_listenFd);
		exit(1);
	}

	
}