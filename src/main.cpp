/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:27:11 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/08 18:57:06 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Epoller.hpp"
#include "Log.hpp"

int main()
{
	Server 	server;
	Epoller	epoller;
	int 	nfds;

	epoller.AddFd(server.GetListenFd(), EPOLLIN | EPOLLET);
	nfds = epoller.Wait(-1);
	if (nfds == -1)
	{
		Log::LogMsg(ERROR, "epoll_wait() failed");
		return 1;
	}
	for (int i = 0; i < nfds; i++)
	{
		int 		sockFd = epoller.GetEventFd(i);
		uint32_t 	events = epoller.GetEvents(i);
		if (epoller.IsServerFd(sockFd) && (events & EPOLLIN)) // new connection
		{
			struct sockaddr_in SockAddr;
			socklen_t SockAddrLen = sizeof(struct sockaddr_in);
			int connFd = accept(sockFd, (struct sockaddr*)&SockAddr, &SockAddrLen);
			if (connFd == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					continue;
				else
				{
					perror("accept() failed");
					continue;
				}
				// close(sockFd);
				// close(epoller.GetEpollFd());
			}
			epoller.AddFd(connFd, EPOLLIN | EPOLLET);
		}
	}

	return 0;
}