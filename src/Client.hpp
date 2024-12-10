/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:42:57 by yrigny            #+#    #+#             */
/*   Updated: 2024/12/10 16:29:53 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Log.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include <map>
# include <vector>
# include <algorithm>
# include <fstream>

using namespace std;

enum ReqStatus
{
	REQ_COMPLETE,
	REQ_INCOMPLETE,
	REQ_BODY_TOO_LARGE,
};

class Server;
class Request;
class Response;

class Client
{
	public:
		Client();
		Client(Server* server, int connFd);
		Client(const Client& src);
		~Client();
		Client&		operator=(const Client& rhs);

		ReqStatus	RecvRequest(string request);
		bool		IsCompleteRequest();
		bool		BodyTooLarge();
		void		ParseRequest();
		void		CheckRequest();
		void		SearchResource();
		void		CheckMethod(vector<HttpMethod> acceptedMethods);
		void		MatchLocation(string requestedUri);
		// void		SearchLocation();
		void		PrepareResponse();
		void		Reset();

		void		SetStatusCode(const string& statusCode);
		void		SetError(bool error);
		void		SetPath(const string& path);
		int			GetConnFd() const;
		string&		GetRequestStr();
		string&		GetResponse();
		bool		GetError() const;
		
	private:
		Server*		_server;
		int			_connFd;
		std::string	_requestStr;
		Request*	_request;
		string		_statusCode;
		bool		_error;
		string		_path;
		Location*	_location;
		Response*	_response;
};


#endif
