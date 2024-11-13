/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:27:11 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/13 15:00:06 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Epoller.hpp"
#include "Log.hpp"

int main()
{
	Server		server;
	vecServer 	cluster;
	cluster.push_back(server);

	Epoller	epoller(cluster);
	epoller.InitEpoller();

	return 0;
}
