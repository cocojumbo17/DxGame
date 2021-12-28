#include "pch.h"
Logger* Logger::s_self = nullptr;

Logger::Logger()
{
	s_self = this;
	m_log_file.open(LogDirectory() + L"/" + LogFile(), std::ios_base::app);
	if (!m_log_file.is_open())
	{
		MessageBox(0, L"Cannot open log file...", L"Log error", MB_OK);
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

void Logger::PrintLog(const wchar_t* fmt, ...)
{
	if (s_self)
	{
		wchar_t buf[MAX_NAME_STRING * 20] = { 0 };
		va_list args;
		va_start(args, fmt);
		vswprintf_s(buf, fmt, args);
		va_end(args);
		s_self->WriteBuffer(buf);
	}
}

void Logger::PrintSeparator()
{
	if (s_self)
	{
		wchar_t buf[] = L"\n------------------------------------------------------------------------------------\n\n";
		s_self->WriteBuffer(buf, false);
	}
}

std::wstring Logger::LogDirectory()
{
	wchar_t path_name[MAX_NAME_STRING * 10];
	GetCurrentDirectory(MAX_NAME_STRING * 10, path_name);
	wcscat_s(path_name, L"\\Logs");
	CreateDirectory(path_name, nullptr);
	return path_name;
}

std::wstring Logger::LogFile()
{
	return std::wstring(L"log") + Time::GetDataTime(true) + L".txt";
}

void Logger::WriteBuffer(const wchar_t* buf, bool with_timestamp)
{
	if (m_log_file.is_open())
	{
		if (with_timestamp)
			m_log_file << L"[" << Time::GetDataTime(false) << L"] ";
		m_log_file << buf << std::endl;
	}
}
