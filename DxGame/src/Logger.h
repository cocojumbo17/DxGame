#pragma once
#include <string>
#include <fstream>

class Logger
{
public:
	Logger();
	~Logger();
	static Logger* GetInstance();
	static void PrintLog(const wchar_t* fmt, ...);
	static void PrintSeparator();
	static std::wstring LogDirectory();
	static std::wstring LogFile();
private:
	void WriteBuffer(const wchar_t* buf, bool with_timestamp = true);
private:
	static Logger* s_self;
	std::wfstream	m_log_file;
};
