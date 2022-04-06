#pragma once

class Log
{
public:
	Log(const char* fileName);
	~Log();

	void Print(const char* text);

private:
	std::fstream m_logFile;
};

void PrintLog(const std::string& text);
inline void ErrorLog(const std::string& text)
{
	PrintLog("ERROR: " + text);
}