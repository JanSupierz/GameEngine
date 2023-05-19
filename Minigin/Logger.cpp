#include "Logger.h"

using namespace dae;

std::unique_ptr<LoggingSystem> Logger::s_Instance{ std::make_unique<NullLoggingSystem>() };

LoggingSystem& Logger::Get()
{
	return *s_Instance;
}

void Logger::RegisterService(std::unique_ptr<LoggingSystem>&& pService)
{
	s_Instance = (pService == nullptr ? std::make_unique<NullLoggingSystem>() : std::move(pService));
}