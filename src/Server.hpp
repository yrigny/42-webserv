/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:16:43 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/08 10:39:55 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include <netinet/in.h> // sockaddr_in
# include <vector>
# include <map>

typedef struct s_location
{
	std::string 				_path;
	std::string 				_root;
	std::string 				_index;
	bool 						_autoIndex;
	std::vector<std::string>	_cgiExtension;
	std::vector<std::string>	_cgiBin;
	std::vector<std::string>	_methods;
	std::string 				_redir;
}	Location;

class Server
{
	public:
		Server();
		~Server();

		void	InitServer();
		void	InitSocket();
		void	EventListen();
		void	EventLoop();

	private:
		uint16_t					_port;
		in_addr_t					_ip;
		std::string 				_serverName;
		std::string 				_root;
		std::vector<std::string> 	_indexes;
		bool 						_autoIndex;
		std::string 				_errorPage;
		std::vector<std::string>	_methods;
		std::vector<Location>		_locations;
		unsigned int 				_maxBodySize;
		std::map<int, std::string>	_errorPages;
		std::map<int, std::string>	_returnURI;
		std::string					_uploadPath;
		std::vector<std::string>	_binPaths;
		std::vector<std::string>	_cgiExtensions;
		struct sockaddr_in			_addr;
		int 						_listenFd;
};


#endif