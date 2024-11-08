/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:55:15 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/08 16:29:24 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

LogState	Log::state = ON;
LogMode		Log::mode = CONSOLE;

std::map<LogLevel, std::string> Log::levelStr;

Log::Log()
{
	levelStr[DEBUG] = "DEBUG  ";
	levelStr[INFO] = "INFO   ";
	levelStr[ERROR] = "ERROR  ";
}

Log::~Log()
{
}

std::string	Log::GetCurrTime()
{
	tzset();
	char		dt[64];
	time_t		now = time(0);
	struct tm*	localTime = localtime(&now);
	strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localTime);
	return dt;
}

void	Log::LogMsg(LogLevel level, std::string msg)
{
	if (state == OFF)
		return;
	std::string	currTime = GetCurrTime();
	if (mode == CONSOLE)
		std::cout << currTime << " " << levelStr[level] << " " << msg << std::endl;
}

void	Log::SetState(std::string state)
{
	if (state == "ON")
		Log::state = ON;
	else if (state == "OFF")
		Log::state = OFF;
}

void	Log::SetMode(std::string mode)
{
	if (mode == "CONSOLE")
		Log::mode = CONSOLE;
	else if (mode == "FILE")
		Log::mode = OUTFILE;
}
