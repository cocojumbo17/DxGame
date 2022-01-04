#include "pch.h"
Logger* Logger::s_self = nullptr;

Logger::Logger()
{
	s_self = this;
	m_log_file.open(LogDirectory() + "/" + LogFile(), std::ios_base::app);
	if (!m_log_file.is_open())
	{
		MessageBoxA(0, "Cannot open log file...", "Log error", MB_OK);
	}

}

Logger::~Logger()
{
	m_log_file.close();
}

Logger* Logger::GetInstance()
{
	return s_self;
}

void Logger::PrintLog(const char* fmt, ...)
{
	if (s_self)
	{
		char buf[MAX_NAME_STRING * 20] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf_s(buf, fmt, args);
		va_end(args);
		s_self->WriteBuffer(buf);
	}
}

void Logger::PrintSeparator()
{
	if (s_self)
	{
		char buf[] = "\n------------------------------------------------------------------------------------\n\n";
		s_self->WriteBuffer(buf, false);
	}
}

std::string Logger::LogDirectory()
{
	char path_name[MAX_NAME_STRING * 10];
	GetCurrentDirectoryA(MAX_NAME_STRING * 10, path_name);
	strcat_s(path_name, "\\Logs");
	CreateDirectoryA(path_name, nullptr);
	return path_name;
}

std::string Logger::LogFile()
{
	return std::string("log") + Time::GetDataTime(true) + ".txt";
}

void Logger::WriteBuffer(const char* buf, bool with_timestamp)
{
	if (m_log_file.is_open())
	{
		if (with_timestamp)
			m_log_file << "[" << Time::GetDataTime(false) << "] ";
		m_log_file << buf << std::endl;
	}
}
