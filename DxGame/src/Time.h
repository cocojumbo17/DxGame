#pragma once
#include <string>
namespace Time
{
	std::string GetTime(bool is_short_format = false);
	std::string GetData(bool is_short_format = false);
	std::string GetDataTime(bool is_short_format = false);
}
