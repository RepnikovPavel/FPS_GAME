#include "LoggingSystem.h"

LoggingSystem::LoggingSystem(std::string EventLogPath)
{
	std::ofstream _EventLog=std::ofstream(EventLogPath,std::ios::trunc);
	_EventLogPath = EventLogPath;
}

LoggingSystem::~LoggingSystem()
{
	_EventLog.close();
	delete _InstancePtr;
#ifdef START_LOG_NOTEPAD_AT_END_OF_PROGRAM
	system((std::string("start notepad ")+_EventLogPath).c_str());
#endif
}

LoggingSystem* LoggingSystem::Instance(std::string EventLogPath)
{
	if (!_InstancePtr)
	{
		_InstancePtr = new LoggingSystem(EventLogPath);
	}
	return _InstancePtr;
}

void LoggingSystem::WriteToLog(std::string SomeText)
{
	_EventLog<<SomeText;
}

LoggingSystem* LoggingSystem::_InstancePtr{nullptr};