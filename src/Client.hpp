/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:42:57 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/20 17:07:16 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Log.hpp"
# include "Request.hpp"
# include <map>
using namespace std;

enum ReqStatus
{
	REQ_COMPLETE,
	REQ_INCOMPLETE,
	REQ_BODY_TOO_LARGE,
};

class Client
{
	public:
		Client();
		Client(int connFd);
		Client(const Client& src);
		~Client();
		Client&		operator=(const Client& rhs);

		ReqStatus	RecvRequest(string request);
		bool		IsCompleteRequest();
		bool		BodyTooLarge();

		int			GetConnFd() const;
		string&		GetRequestStr();
		
	private:
		int			_connFd;
		std::string	_requestStr;
};


#endif
