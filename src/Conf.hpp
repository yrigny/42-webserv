/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:26:27 by yrigny            #+#    #+#             */
/*   Updated: 2024/12/09 14:12:04 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_HPP
# define CONF_HPP
# include "Log.hpp"
# include <string>
# include <vector>
# include <map>
# include <cstdlib>
# include <netinet/in.h> // sockaddr_in
# include <arpa/inet.h> // inet_addr, inet_ntoa
using namespace std;

enum HttpMethod
{
	GET,
	POST,
	DELETE,
};

typedef struct s_location
{
	std::string 				_path;
	std::string 				_root;
	std::string 				_index;
	bool 						_autoIndex;
	std::string					_cgiExtension;
	std::string					_cgiBin;
	std::vector<HttpMethod>		_methods;
	std::string 				_uploadPath;
	std::map<int, std::string>	_errorPages;
	std::map<int, std::string> 	_returnUri;
}	Location;

typedef struct s_server
{
	string				listen;
	string				serverName;
	string				host;
	string				clientMaxBodySize;
	string				root;
	map<int, string>	errorPages;
	string				indexes;
	string				autoIndex;
	vector<string>		methods;
	string				uploadDir;
	string				cgiBin;
	string				cgiExtension;
	map<int, string>	returnURI;
	vector<Location>	locations;
}	ServerInfo;

class Conf
{
	public:
		Conf(const string path);
		~Conf();
	
		vector<ServerInfo>	GetServerInfos();
		size_t				GetNbServers();

	private:
		vector<ServerInfo>	_serverInfos;
		int					_nbServers;
				
		void				_ParseServers(ifstream &file);
		bool				_FindServerBlockStart(string line);
		bool				_FindServerBlockEnd(string line);
		bool				_FindLocationBlockStart(string line);
		bool				_FindLocationBlockEnd(string line);
		void				_ParseLocationBlock(string firstline, ifstream &file, ServerInfo *serverInfo);
		void				_ParseLine(string line, ServerInfo *serverInfo);
		void				_ParseLocationLine(string line, Location *location);
		void				_SplitToTokens(string line, vector<string>& tokens);
		bool				_SplitToKeyValuePair(string line, string &key, string &value);
		bool				_CheckServerInfo(ServerInfo serverInfo);
};

#endif
