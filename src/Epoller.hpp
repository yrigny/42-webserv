/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoller.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:43:32 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/07 14:29:31 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLER_HPP
# define EPOLLER_HPP

#include "Log.hpp"
#include <sys/epoll.h> // epoll_create1, epoll_ctl, epoll_wait
#include <vector>
#include <fcntl.h> // fcntl
#include <unistd.h> // close
#include <cerrno> // errno
#include <cstdio> // perror
#include <cstdlib> // exit

class Epoller
{
	public:
		explicit Epoller(int maxEvent = 1024);
		~Epoller();
		
		bool		AddFd(int fd, uint32_t events);
		bool		ModFd(int fd, uint32_t events);
		bool		DelFd(int fd);
		int			Wait(int timeoutMs = -1);
		int			GetEventFd(size_t i) const;
		uint32_t	GetEvents(size_t i) const;
	
	private:
		int _epollFd;
		std::vector<struct epoll_event> _events;
};

#endif