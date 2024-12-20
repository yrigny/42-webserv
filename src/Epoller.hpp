/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoller.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:43:32 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/21 17:20:07 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLER_HPP
# define EPOLLER_HPP

# include "Log.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include <sys/epoll.h> // epoll_create1, epoll_ctl, epoll_wait
# include <vector>
# include <unistd.h> // close
# include <cerrno> // errno
# include <cstdio> // perror
# include <cstdlib> // exit
# include <fcntl.h> // fcntl
# include <string>
# include <sstream>

extern bool	run;

typedef std::vector<struct epoll_event> VecEvent;

class Epoller
{
	public:
		Epoller(VecServer& servers, int maxEvent = 1024);
		~Epoller();
		
		void		InitEpoller();
		bool		AddServerSockets();
		bool		AddFd(int fd, uint32_t events);
		bool		ModFd(int fd, uint32_t events);
		bool		DelFd(int fd);
		int			MatchListenFd(int fd);
		int			MatchClientFd(int fd);
		int			EpollWait(int timeoutMs = -1);
		void		SafeExit();
		bool		InitConnection(int sockFd, int serverIdx);
		bool		RequestTransfer(int connFd, int serverIdx);
		bool		SetNonBlocking(int connFd);
		int			GetEventFd(size_t i) const;
		uint32_t	GetEvents(size_t i) const;
		std::string	EventToStr(uint32_t events) const;
	
	private:
		int 				_epollFd;
		VecEvent 			_events;
		VecServer&			_servers;
};

#endif