/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:36:33 by yrigny            #+#    #+#             */
/*   Updated: 2024/12/09 15:14:30 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Conf.hpp"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

bool	FileExists(const string path)
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

bool	IsDirectory(const string path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return false;
	return S_ISDIR(statbuf.st_mode);
}

bool	IsConfFile(const string path)
{
	return path.substr(path.find_last_of(".") + 1) == "conf";
}

bool	IsReadable(const string path)
{
	return access(path.c_str(), R_OK) == 0;
}

Conf::Conf(const string path) : _serverInfos(), _nbServers(0)
{
	if (path.empty())
		throw std::invalid_argument("Config path is empty");
	if (!FileExists(path))
		throw std::invalid_argument("Config path does not exist");
	if (IsDirectory(path))
		throw std::invalid_argument("Config path is a directory");
	if (!IsConfFile(path))
		throw std::invalid_argument("Config path is not a .conf file");
	if (!IsReadable(path))
		throw std::invalid_argument("Config file is not readable");
	ifstream file(path.c_str());
	if (!file.is_open())
		throw std::invalid_argument("Config file could not be opened");
	this->_ParseServers(file);
}

Conf::~Conf()
{
}

void	Conf::_ParseServers(ifstream &file)
{
	string line;
	int openBracket = 0;

	while (getline(file, line))
	{
		if (line.empty() || line[0] == '#' || line[0] == ';')
            continue;
		if (_FindServerBlockStart(line))
		{
			openBracket = 1;
			ServerInfo serverInfo;
			// parse the server block
			while (getline(file, line))
			{
				if (line.empty() || line[0] == '#' || line[0] == ';')
					continue;
				if (_FindServerBlockEnd(line))
				{
					openBracket = 0;
					// check if the serverinfo is valid
					bool	valid = _CheckServerInfo(serverInfo);
					if (!valid)
						throw std::invalid_argument("Invalid server block");
					_serverInfos.push_back(serverInfo);
					_nbServers++;
					break;
				}
				if (_FindLocationBlockStart(line))
					_ParseLocationBlock(line, file, &serverInfo);
				else
					_ParseLine(line, &serverInfo);
			}
			if (openBracket == 1)
				throw std::invalid_argument("Invalid server block");
		}
	}
	if (_nbServers == 0)
		throw std::invalid_argument("No valid server blocks found");
}

bool	Conf::_CheckServerInfo(ServerInfo serverInfo)
{
	/* check obligatory fields of serverInfo: listen, serverName */
	// listen should be a number between 1 and 65535 and not repeated
	int	port = std::atoi(serverInfo.listen.c_str());
	if (port < 1 || port > 65535)
		return false;
	for (size_t i = 0; i < _serverInfos.size(); i++)
	{
		if (_serverInfos[i].listen.compare(serverInfo.listen) == 0)
			return false;
	}
	// serverName should not be empty
	if (serverInfo.serverName.empty())
		return false;
	return true;
}

bool	Conf::_FindServerBlockStart(string line)
{
	// use stringstream to verify the pattern "server {"
	istringstream	iss(line);
	string			server_keyword, open_bracket;
	if (!(iss >> server_keyword >> open_bracket))
		return false;
	if (server_keyword != "server" || open_bracket != "{")
		return false;
	// verify if there are no characters after "{"
	string	rest;
	if (iss >> rest)
		return false;
	return true;
}

bool	Conf::_FindServerBlockEnd(string line)
{
	// use stringstream to verify the pattern "}"
	istringstream	iss(line);
	string			close_bracket;
	if (!(iss >> close_bracket))
		return false;
	if (close_bracket != "}")
		return false;
	// verify if there are no characters after "}"
	string	rest;
	if (iss >> rest)
		return false;
	return true;
}

bool	Conf::_FindLocationBlockStart(string line)
{
	// use stringstream to verify the pattern "location /path {"
	istringstream	iss(line);
	string			location_keyword, path, open_bracket;
	if (!(iss >> location_keyword >> path >> open_bracket))
		return false;
	if (location_keyword != "location" || path.find('/') == string::npos || open_bracket != "{")
		return false;
	// verify if there are no characters after "{"
	string	rest;
	if (iss >> rest)
		return false;
	return true;
}

bool	Conf::_FindLocationBlockEnd(string line)
{
	// use stringstream to verify the pattern "}"
	istringstream	iss(line);
	string			close_bracket;
	if (!(iss >> close_bracket))
		return false;
	if (close_bracket != "}")
		return false;
	// verify if there are no characters after "}"
	string	rest;
	if (iss >> rest)
		return false;
	return true;
}

void	Conf::_ParseLocationBlock(string firstline, ifstream &file, ServerInfo *serverInfo)
{
	string line;
	Location location;
	int openBracket = 1;

	// parse the uri from the first line "location /path {"
	stringstream	ss(firstline);
	string			location_keyword, uri;
	ss >> location_keyword >> uri;
	location._path = uri;
	// parse the location block
	while (getline(file, line))
	{
		if (line.empty() || line[0] == '#' || line[0] == ';')
			continue;
		if (_FindLocationBlockEnd(line))
		{
			openBracket = 0;
			serverInfo->locations.push_back(location);
			break;
		}
		_ParseLocationLine(line, &location);
	}
	if (openBracket == 1)
		throw std::invalid_argument("Invalid location block");
}

void	Conf::_ParseLine(string line, ServerInfo *serverInfo)
{
	string::size_type pos = line.find(";");
	if (pos == string::npos)
		throw std::invalid_argument("Invalid line");
	// remove the ";" from the line
	line = line.substr(0, pos);

	string key;
	string value;
	if (!_SplitToKeyValuePair(line, key, value))
		throw std::invalid_argument("Invalid line");
	if (key.compare("listen") == 0)
		serverInfo->listen = value;
	else if (key.compare("server_name") == 0)
		serverInfo->serverName = value;
	else if (key.compare("host") == 0)
		serverInfo->host = value;
	else if (key.compare("client_max_body_size") == 0)
		serverInfo->clientMaxBodySize = value;
	else if (key.compare("root") == 0)
		serverInfo->root = value;
	else if (key.compare("error_page") == 0)
	{
		istringstream iss(value);
		string code, path;
		iss >> code >> path;
		serverInfo->errorPages[std::atoi(code.c_str())] = path;
	}
	else if (key.compare("index") == 0)
		serverInfo->indexes = value;
	else if (key.compare("autoindex") == 0)
		serverInfo->autoIndex = value;
	else if (key.compare("upload_path") == 0)
		serverInfo->uploadDir = value;
	else if (key.compare("allowedMethods") == 0)
	{
		istringstream iss(value);
		string method;
		while (iss >> method)
			serverInfo->methods.push_back(method);
	}
	else if (key.compare("cgi_bin") == 0)
		serverInfo->cgiBin = value;
	else if (key.compare("cgi_extension") == 0)
		serverInfo->cgiExtension = value;
	else if (key.compare("return") == 0)
	{
		stringstream ss(value);
		string code, uri;
		ss >> code >> uri;
		serverInfo->returnURI[std::atoi(code.c_str())] = uri;
	}
	else
		throw std::invalid_argument("Invalid line");
}

void	Conf::_ParseLocationLine(string line, Location *location)
{
	string::size_type pos = line.find(";");
	if (pos == string::npos)
		throw std::invalid_argument("Invalid line");
	// remove the ";" from the line
	line = line.substr(0, pos);
	// split the line into tokens
	vector<string> tokens;
	_SplitToTokens(line, tokens);
	if (tokens.size() < 2)
		throw std::invalid_argument("Invalid line");
	// parse the key-value pair
	string key = tokens[0];
	if (key.compare("root") == 0)
	{
		if (!IsDirectory(tokens[1]))
			throw std::invalid_argument("Invalid root directory");
		if (tokens[1][tokens[1].size() - 1] != '/')
			tokens[1] += '/';
		location->_root = tokens[1];
	}
	else if (key.compare("index") == 0)
	{
		string fullPath = location->_root + tokens[1];
		if (!FileExists(fullPath))
			throw std::invalid_argument("Invalid index file");
		location->_index = tokens[1];
	}
	else if (key.compare("autoindex") == 0)
	{
		if (tokens[1].compare("on") != 0 && tokens[1].compare("off") != 0)
			throw std::invalid_argument("Invalid autoindex value");
		location->_autoIndex = tokens[1] == "on" ? true : false;
	}
	else if (key.compare("cgi_bin") == 0)
	{
		if (!FileExists(tokens[1]))
			throw std::invalid_argument("Invalid cgi-bin directory");
		location->_cgiBin = tokens[1];
	}
	else if (key.compare("cgi_extension") == 0)
	{
		if (tokens[1].find('.') == string::npos)
			throw std::invalid_argument("Invalid cgi extension");
		location->_cgiExtension = tokens[1];
	}
	else if (key.compare("allowedMethods") == 0)
	{
		for (size_t i = 1; i < tokens.size(); i++)
		{
			if (tokens[i].compare("GET") == 0)
				location->_methods.push_back(GET);
			else if (tokens[i].compare("POST") == 0)
				location->_methods.push_back(POST);
			else if (tokens[i].compare("DELETE") == 0)
				location->_methods.push_back(DELETE);
			else
				throw std::invalid_argument("Invalid method");
		}
	}
	else if (key.compare("upload_path") == 0)
	{
		if (!IsDirectory(tokens[1]))
			throw std::invalid_argument("Invalid upload directory");
		location->_uploadPath = tokens[1];
	}
	else if (key.compare("error_page") == 0)
	{
		int code = std::atoi(tokens[1].c_str());
		if (code < 400 || code > 599)
			throw std::invalid_argument("Invalid error code");
		string fullPath = location->_root + tokens[2];
		if (!FileExists(fullPath))
			throw std::invalid_argument("Invalid error page");
		location->_errorPages[code] = tokens[2];
	}
	else if (key.compare("return") == 0)
	{
		int code = std::atoi(tokens[1].c_str());
		if (code < 300 || code > 399)
			throw std::invalid_argument("Invalid return code");
		location->_returnUri[std::atoi(tokens[1].c_str())] = tokens[2];
	}
	else
		throw std::invalid_argument("Invalid line");
}

void	Conf::_SplitToTokens(string line, vector<string>& tokens)
{
	istringstream iss(line);
	string word;

	while (iss >> word)
		tokens.push_back(word);
}

bool	Conf::_SplitToKeyValuePair(string line, string& key, string& value)
{
	istringstream iss(line);
	string word;

	if (!(iss >> word))
		return false;
	key = word;
	if (!(iss >> word))
		return false;
	size_t pos = line.find(word);
	value = line.substr(pos);
	return true;
}

vector<ServerInfo>	Conf::GetServerInfos()
{
	return this->_serverInfos;
}

size_t	Conf::GetNbServers()
{
	return this->_nbServers;
}
