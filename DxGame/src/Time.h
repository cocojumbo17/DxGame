#pragma once
#include <string>
namespace Time
{
	std::wstring GetTime(bool is_short_format = false);
	std::wstring GetData(bool is_short_format = false);
	std::wstring GetDataTime(bool is_short_format = false);
}
