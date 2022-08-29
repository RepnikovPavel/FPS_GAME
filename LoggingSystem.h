#pragma once
#include "CoreMinimal.h"

#define LOG_FILE_PATH "C:\\FPS_LOG\\Log.txt"

class FPS_API LoggingSystem
{
private:
	LoggingSystem(const char* EventLogPath);
	static LoggingSystem* _InstancePtr;
public:
	static LoggingSystem* Instance(const char* EventLogPath);
public:
	void WriteToLog(std::string SomeText) const;
public:
	~LoggingSystem();
private:
	std::string _EventLogPath;
};
