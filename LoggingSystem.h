#pragma once
#include "CoreMinimal.h"
#include <fstream>

#define LOG_FILE_PATH "C:\\FPS_LOG\\Log.txt"

#define START_LOG_NOTEPAD_AT_END_OF_PROGRAM

class FPS_API LoggingSystem
{
private:
	LoggingSystem(std::string EventLogPath);
	static LoggingSystem* _InstancePtr;
public:
	static LoggingSystem* Instance(std::string EventLogPath);
public:
	void WriteToLog(std::string);
public:
	~LoggingSystem();
private:
	std::ofstream _EventLog;
	std::string _EventLogPath;
};
