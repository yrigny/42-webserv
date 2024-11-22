/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:42:57 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/22 19:29:00 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Log.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include <map>
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
		void		SearchLocation();
		void		PrepareResponse();

		int			GetConnFd() const;
		string&		GetRequestStr();
		string&		GetResponse();
		
	private:
		Server*		_server;
		int			_connFd;
		std::string	_requestStr;
		Request*	_request;
		int			_statusCode;
		Response*	_response;
};


#endif
