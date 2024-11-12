/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoller.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:50:20 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/12 20:33:59 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoller.hpp"

Epoller::Epoller(vecServer& servers, int maxEvent) : _epollFd(epoll_create1(0)), _events(maxEvent), _servers(servers)
{
	if (_epollFd == -1)
	{
		Log::LogMsg(ERROR, "epoll_create1() failed");
		exit(1);
	}
	_serverFds.clear();
}

Epoller::~Epoller()
{
	close(_epollFd);
}

void	Epoller::InitEpoller(vecServer& servers)
{
	if (!this->AddServerSockets(servers))
	{
		Log::LogMsg(ERROR, "AddServerSockets() failed");
		close(_epollFd);
		exit(1);
	}
	// if (!this->AddFd(STDIN_FILENO, EPOLLIN))
	// {
	// 	Log::LogMsg(ERROR, "AddFd() failed");
	// 	close(_epollFd);
	// 	exit(1);
	// }
	Log::LogMsg(INFO, "Start webserv...");
	while (this->EpollWait(-1) != -1)
		;
	Log::LogMsg(INFO, "Stop webserv...");
	for (size_t i = 0; i < _serverFds.size(); i++)
	{
		close(_serverFds[i]);
		this->DelFd(_serverFds[i]);
	}
}

bool	Epoller::AddServerSockets(vecServer& servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		std::cout << servers[i];
		if (!AddFd(servers[i].GetListenFd(), EPOLLIN | EPOLLET))
			return false;
	}
	_servers = servers;
	return true;
}

bool	Epoller::AddFd(int fd, uint32_t events)
{
	if (fd < 0)
		return false;
	struct epoll_event event;
	event.data.fd = fd;
	event.events = events;
	_serverFds.push_back(fd);
	std::cout << "fd: " << fd << ", events: " << EventToStr(events) << std::endl;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event) -1)
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
	struct epoll_event event;
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
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		Log::LogMsg(ERROR, "epoll_ctl() failed");
		close(fd);
		close(_epollFd);
		exit(1);
	}
	return true;
}

int	Epoller::MatchServerFd(int fd)
{
	for (size_t i = 0; i < _serverFds.size(); i++)
	{
		if (_serverFds[i] == fd)
			return i;
	}
	return -1;
}

int		Epoller::EpollWait(int timeoutMs)
{
	int nfds;

	if ((nfds = epoll_wait(_epollFd, &_events[0], static_cast<int>(_events.size()), timeoutMs)) == -1)
	{
		Log::LogMsg(ERROR, "epoll_wait() failed");
		return -1;
	}
	for (int i = 0; i < nfds; i++)
	{
		int sockFd = _events[i].data.fd;
		if ((_events[i].events & EPOLLERR) || (_events[i].events & EPOLLHUP) || !(_events[i].events & EPOLLIN))
		{
			Log::LogMsg(ERROR, "epoll error, events: " + Log::ToString(_events[i].events));
			close(sockFd);
			return 0;
		}
		int serverIdx = this->MatchServerFd(sockFd);
		if ((_events[i].events & EPOLLIN) && serverIdx > 0) // new connection
		{
			if (!this->InitConnection(sockFd, serverIdx))
				return -1;
		}
		else // existing connection
		{
			if (!this->RequestHandler(sockFd, serverIdx))
				return -1;
		}
	}
	return 0;
}

bool	Epoller::InitConnection(int sockFd, int serverIdx)
{
	struct sockaddr_in sockAddr;
	socklen_t sockAddrLen = sizeof(sockAddr);
	Log::LogMsg(INFO, "New connection at port " + Log::ToString(_servers[serverIdx].GetPort()));
	
	int connFd = accept(sockFd, (struct sockaddr*)&sockAddr, &sockAddrLen);
	if (connFd == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return true;
		else
		{
			Log::LogMsg(ERROR, "accept() failed");
			return false;
		}
	}
	if (!this->SetNonBlocking(connFd))
		return false;
	_servers[serverIdx].SetClientConnection(connFd);
	
	Log::LogMsg(INFO, "New socket " + Log::ToString(connFd) + " created for communication");
	if (!this->AddFd(connFd, EPOLLIN))
		return false;
	return true;
}

bool	Epoller::SetNonBlocking(int connFd)
{
	int oldOption = fcntl(connFd, F_GETFL);
	int newOption = oldOption | O_NONBLOCK;
	if (fcntl(connFd, F_SETFL, newOption) == -1)
	{
		Log::LogMsg(ERROR, "fcntl() failed");
		return false;
	}
	return true;
}

bool	Epoller::RequestHandler(int connFd, int serverIdx)
{
	Log::LogMsg(INFO, "RequestHandler() called");
	std::cout << "connFd: " << connFd << ", serverIdx: " << serverIdx << std::endl;
	return true;
}

int		Epoller::GetEventFd(size_t i) const
{
	return _events[i].data.fd;
}

uint32_t	Epoller::GetEvents(size_t i) const
{
	return _events[i].events;
}

std::string	Epoller::EventToStr(uint32_t events) const
{
	// uint32_t events = _events[i].events;
	std::string str;
	if (events & EPOLLIN)
		str += "EPOLLIN ";
	if (events & EPOLLOUT)
		str += "EPOLLOUT ";
	if (events & EPOLLRDHUP)
		str += "EPOLLRDHUP ";
	if (events & EPOLLPRI)
		str += "EPOLLPRI ";
	if (events & EPOLLERR)
		str += "EPOLLERR ";
	if (events & EPOLLHUP)
		str += "EPOLLHUP ";
	if (events & EPOLLET)
		str += "EPOLLET ";
	if (events & EPOLLONESHOT)
		str += "EPOLLONESHOT ";
	return str;
}
