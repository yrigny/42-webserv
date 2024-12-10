/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:56:39 by yrigny            #+#    #+#             */
/*   Updated: 2024/12/10 15:28:05 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Server.hpp"
# include "Client.hpp"
using namespace std;

typedef struct s_header
{
	map<string, string>	_fields;
}	Header;

class Request
{
	public:
		Request();
		~Request();

		void	SetMethod(string method);
		void	SetUri(string uri);
		void	SetVersion(string version);
		void	SetHeaderField(string field, string value);
		void	SetBody(string body);
		string	GetMethod() const;
		string	GetUri() const;
		string	GetVersion() const;
		string*	GetBody() const;
		string	GetHeaderField(string field) const;

	private:
		string	_method;
		string	_uri;
		string	_version;
		Header	_header;
		string	_body;
};

#endif
