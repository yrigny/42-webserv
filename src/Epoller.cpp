/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoller.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:50:20 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/07 14:35:33 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoller.hpp"

Epoller::Epoller(int maxEvent) : _epollFd(epoll_create1(0)), _events(maxEvent)
{
	if (_epollFd == -1)
	{
		Log::LogMsg(ERROR, "epoll_create1() failed");
		exit(1);
	}
}

Epoller::~Epoller()
{
	close(_epollFd);
}

bool	Epoller::AddFd(int fd, uint32_t events)
{
	if (fd < 0)
		return false;
	struct epoll_event event = {0};
	event.data.fd = fd;
	event.events = events;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		Log::LogMsg(ERROR, "epoll_ctl() failed");
		close(fd);
		close(_epollFd);
		exit(1);
	}
	return true;
}

bool	Epoller::ModFd(int fd, uint32_t events)
{
	if (fd < 0)
		return false;
	struct epoll_event event = {0};
	event.data.fd = fd;
	event.events = events;
	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event) == -1)
	{
		Log::LogMsg(ERROR, "epoll_ctl() failed");
		close(fd);
		close(_epollFd);
		exit(1);
	}
	return true;
}

bool	Epoller::DelFd(int fd)
{
	if (fd < 0)
		return false;
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
	{
		Log::LogMsg(ERROR, "epoll_ctl() failed");
		close(fd);
		close(_epollFd);
		exit(1);
	}
	return true;
}

int		Epoller::Wait(int timeoutMs)
{
	return epoll_wait(_epollFd, &_events[0], static_cast<int>(_events.size()), timeoutMs);
}

int		Epoller::GetEventFd(size_t i) const
{
	return _events[i].data.fd;
}

uint32_t	Epoller::GetEvents(size_t i) const
{
	return _events[i].events;
}

