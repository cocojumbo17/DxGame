#include "pch.h"
#include "Time.h"
#include <time.h>
#include <sstream>
#include <iomanip>

namespace Time
{
	std::wstring GetTime(bool is_short_format)
	{
		time_t now = time(nullptr);
		tm ltm;
		localtime_s(&ltm, &now);
		std::wstringstream wss;
		if (is_short_format)
			wss << std::setw(2) << std::setfill(L'0') << ltm.tm_hour << std::setw(2) << std::setfill(L'0') << ltm.tm_min << std::setw(2) << std::setfill(L'0') << ltm.tm_sec;
		else
			wss << std::setw(2) << std::setfill(L'0') << ltm.tm_hour << L":" << std::setw(2) << std::setfill(L'0') << ltm.tm_min << L":" << std::setw(2) << std::setfill(L'0') << ltm.tm_sec;

		return wss.str();
	}

	std::wstring GetData(bool is_short_format)
	{
		time_t now = time(nullptr);
		tm ltm;
		localtime_s(&ltm, &now);
		std::wstringstream wss;
		wss.width(2);
		wss.fill(L'0');
		if (is_short_format)
			wss << ltm.tm_year + 1900 << std::setw(2) << std::setfill(L'0') << ltm.tm_mon << std::setw(2) << std::setfill(L'0') << ltm.tm_mday;
		else
			wss << ltm.tm_year + 1900 << L"/" << std::setw(2) << std::setfill(L'0') << ltm.tm_mon << L"/" << std::setw(2) << std::setfill(L'0') << ltm.tm_mday;

		return wss.str();
	}

	std::wstring GetDataTime(bool is_short_format)
	{
		std::wstring res = GetData(is_short_format);
		if (!is_short_format)
			res += L" ";
		res += GetTime(is_short_format);
		return res;
	}
}