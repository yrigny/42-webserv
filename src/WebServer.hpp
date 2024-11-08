/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:45:13 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/07 14:49:26 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include "Epoller.hpp"
# include <sys/socket.h> // socket, bind, listen, accept
# include <netinet/in.h> // sockaddr_in
# include <cstring> // memset


class WebServer
{
	public:
		WebServer();
		~WebServer();

		void	InitServer();
		void	InitSocket();
		void	EventListen();
		void	EventLoop();

	private:
		int 	_listenFd;
		Epoller _epoller;
		// http_conn
		// root path
		// timer
};

#endif
