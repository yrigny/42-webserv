/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:27:11 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/12 19:35:07 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Epoller.hpp"
#include "Log.hpp"

int main()
{
	vecServer 	cluster;
	cluster.push_back(Server());

	Epoller	epoller(cluster);
	epoller.InitEpoller(cluster);

	return 0;
}