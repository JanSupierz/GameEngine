#pragma once
#include <sstream>
#include <iostream>
#include "ServiceLocator.h"

class LoggingSystem
{
public:
	virtual ~LoggingSystem() = default;
	virtual void Log(const std::string& message) = 0;
};

template <typename Service>
class NullService;

template <>
class NullService<LoggingSystem> : public LoggingSystem {
public:
#if _DEBUG
	virtual void Log(const std::string& message) { std::cout << "Log: " << message << '\n'; };
#else
	virtual void Log(const std::string&) {};
#endif
};