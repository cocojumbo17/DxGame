#include "pch.h"
#include "Time.h"
#include <time.h>
#include <iomanip>

namespace Time
{
	std::string GetTime(bool is_short_format)
	{
		time_t now = time(nullptr);
		tm ltm;
		localtime_s(&ltm, &now);
		std::stringstream wss;
		if (is_short_format)
			wss << std::setw(2) << std::setfill('0') << ltm.tm_hour << std::setw(2) << std::setfill('0') << ltm.tm_min << std::setw(2) << std::setfill('0') << ltm.tm_sec;
		else
			wss << std::setw(2) << std::setfill('0') << ltm.tm_hour << ":" << std::setw(2) << std::setfill('0') << ltm.tm_min << ":" << std::setw(2) << std::setfill('0') << ltm.tm_sec;

		return wss.str();
	}

	std::string GetData(bool is_short_format)
	{
		time_t now = time(nullptr);
		tm ltm;
		localtime_s(&ltm, &now);
		std::stringstream wss;
		wss.width(2);
		wss.fill(L'0');
		if (is_short_format)
			wss << ltm.tm_year + 1900 << std::setw(2) << std::setfill('0') << ltm.tm_mon << std::setw(2) << std::setfill('0') << ltm.tm_mday;
		else
			wss << ltm.tm_year + 1900 << "/" << std::setw(2) << std::setfill('0') << ltm.tm_mon << "/" << std::setw(2) << std::setfill('0') << ltm.tm_mday;

		return wss.str();
	}

	std::string GetDataTime(bool is_short_format)
	{
		std::string res = GetData(is_short_format);
		if (!is_short_format)
			res += " ";
		res += GetTime(is_short_format);
		return res;
	}
}