/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <yrigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:44:05 by yrigny            #+#    #+#             */
/*   Updated: 2024/11/07 14:38:58 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# include <iostream>
# include <string>
# include <map>

enum LogState
{
	ON,
	OFF,
};

enum LogLevel
{
	DEBUG,
	INFO,
	ERROR,
};

enum LogMode
{
	CONSOLE,
	FILE,
};

class Log
{
	public:
		static LogState	state;
		static LogMode	mode;
		static std::map<LogLevel, std::string> levelStr;

		static std::string	GetCurrTime();
		static void		LogMsg(LogLevel level, std::string msg);
		static void		SetState(std::string state);
		static void		SetMode(std::string mode);
};

#endif
