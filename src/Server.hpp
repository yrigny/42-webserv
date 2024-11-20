/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:16:43 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/20 11:51:08 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Conf.hpp"
# include <iostream>
# include <ostream>
# include <netinet/in.h> // sockaddr_in
# include <arpa/inet.h> // inet_addr, inet_ntoa
# include <cstring> // memset
# include <unistd.h> // close
# include <fcntl.h> // fcntl
# include <cstdlib> // exit, strtol
# include <string> // stoi
# include <vector>
# include <map>
using namespace std;

class Server
{
	public:
		Server();
		Server(ServerInfo& serverInfo);
		~Server();
		Server&	operator=(Server const &rhs);

		void	SetPort(std::string port);
		void	SetHost(std::string host);
		void	SetServerName(std::string serverName);
		void	SetRoot(std::string root);
		void	SetIndexes(std::string indexes);
		void	SetAutoIndex(std::string autoIndex);
		void	SetMethods(std::string methods);
		void	SetLocations(std::string locations);
		void	SetMaxBodySize(std::string maxBodySize);
		void	SetErrorPages(std::string errorPages);
		void	SetUploadPath(std::string uploadPath);
		// void	SetReturnURI(std::string returnURI);
		// void	SetBinPaths(std::string binPaths);
		// void	SetCgiExtensions(std::string cgiExtensions);
		void	InitServer();
		void	CreateSocket();
		void	SetSockAddr();
		void	SetReuseAddr();
		void	SetBind();
		void	SetNonBlocking();
		void	SetListen();
		void	SetClientConnection(int connFd);
		int		HandleRequest(int connFd);
		
		uint16_t					GetPort() const;
		in_addr_t					GetHost() const;
		std::string					GetHostStr() const;
		std::string					GetServerName() const;
		std::string					GetRoot() const;
		std::vector<std::string>	GetIndexes() const;
		bool						GetAutoIndex() const;
		std::vector<HttpMethod>		GetMethods() const;
		std::vector<Location>		GetLocations() const;
		unsigned int				GetMaxBodySize() const;
		std::map<int, std::string>	GetErrorPages() const;
		std::string					GetUploadPath() const;
		int							GetListenFd() const;
		struct sockaddr_in			GetSockAddr() const;
		int							GetConnFd() const;

	private:
		uint16_t					_port;
		in_addr_t					_host;
		std::string 				_serverName;
		std::string 				_root;
		std::vector<std::string> 	_indexes;
		bool 						_autoIndex;
		std::vector<HttpMethod>		_methods;
		std::vector<Location>		_locations;
		unsigned int 				_maxBodySize;
		std::map<int, std::string>	_errorPages;
		std::string					_uploadPath;
		// std::map<int, std::string>	_returnURI;
		// std::vector<std::string>	_binPaths;
		// std::vector<std::string>	_cgiExtensions;
		int 						_listenFd;
		struct sockaddr_in			_sockAddr;
		int 						_connFd;
};

typedef std::vector<Server> VecServer;

std::ostream &operator<<(std::ostream &o, Server const &i);

#endif