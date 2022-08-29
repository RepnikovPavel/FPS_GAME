#include "LoggingSystem.h"
#include <fstream>

LoggingSystem::LoggingSystem(const char* EventLogPath)
{
	std::ofstream _EventLog(EventLogPath,std::ios_base::trunc);
	_EventLog.close();
	_EventLogPath = EventLogPath;
}

LoggingSystem::~LoggingSystem()
{
	delete _InstancePtr;
}

LoggingSystem* LoggingSystem::Instance(const char* EventLogPath)
{
	if (!_InstancePtr)
	{
		_InstancePtr = new LoggingSystem(EventLogPath);
	}
	return _InstancePtr;
}

void LoggingSystem::WriteToLog(std::string SomeText) const
{
	std::ofstream _EventLog(_EventLogPath, std::ios_base::app);
	_EventLog<<SomeText;
	_EventLog.close();
}

LoggingSystem* LoggingSystem::_InstancePtr{nullptr};