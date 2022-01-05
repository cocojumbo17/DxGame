#pragma once
#include <fstream>

class Logger
{
public:
	Logger();
	~Logger();
	static Logger* GetInstance();
	static void PrintLog(const char* fmt, ...);
	static void PrintSeparator();
	static std::string LogDirectory();
	static std::string LogFile();
private:
	void WriteBuffer(const char* buf, bool with_timestamp = true);
private:
	static Logger* s_self;
	std::fstream	m_log_file;
};
