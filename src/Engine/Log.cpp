#include "stdafx.h"
#include "Log.h"
//-----------------------------------------------------------------------------
#define LOG_SEPERATOR                                                            \
    "**************************************************************************" \
    "******************************"
//-----------------------------------------------------------------------------
static Log* thisLog = nullptr;
//-----------------------------------------------------------------------------
Log::Log(const char* fileName)
{
	assert(!thisLog);
	thisLog = this;

	m_logFile.open(fileName, std::ios::out);

	std::time_t rawtime = time(nullptr);
	char str[26];
	ctime_s(str, sizeof str, &rawtime);

	Print(LOG_SEPERATOR);
	Print((str + std::string("Log Started.")).c_str());
	Print(LOG_SEPERATOR);
	Print("");
}
//-----------------------------------------------------------------------------
Log::~Log()
{
	std::time_t rawtime = time(nullptr);
	char str[26];
	ctime_s(str, sizeof str, &rawtime);
	Print("");
	Print(LOG_SEPERATOR);
	Print((str + std::string("Log Ended.")).c_str());
	Print(LOG_SEPERATOR);

	if (m_logFile.good())
	{
		m_logFile.flush();
		m_logFile.close();
	}
}
//-----------------------------------------------------------------------------
void Log::Print(const char* text)
{
	puts(text);

	if (m_logFile.good())
	{
		m_logFile << text << std::endl;
		m_logFile.flush();
	}
}
//-----------------------------------------------------------------------------
void PrintLog(const std::string& text)
{
	assert(thisLog);
	thisLog->Print(text.c_str());
}
//-----------------------------------------------------------------------------