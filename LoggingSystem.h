#pragma once
#include "CoreMinimal.h"
#include <fstream>

#define LOG_FILE_PATH "C:\\FPS_LOG\\Log.txt"

#define START_LOG_NOTEPAD_AT_END_OF_PROGRAM

class FPS_API LoggingSystem
{
private:
	LoggingSystem(const char* EventLogPath);
	static LoggingSystem* _InstancePtr;
public:
	static LoggingSystem* Instance(const char* EventLogPath);
public:
	void WriteToLog(std::string SomeText);
public:
	~LoggingSystem();
private:
	std::string _EventLogPath;
};
