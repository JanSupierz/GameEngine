#pragma once
#include <sstream>
#include <iostream>

namespace dae
{
	class LoggingSystem
	{
	public:
		LoggingSystem() = default;
		virtual ~LoggingSystem() = default;

		LoggingSystem(const LoggingSystem& other) = delete;
		LoggingSystem(LoggingSystem&& other) = delete;
		LoggingSystem& operator=(const LoggingSystem& other) = delete;
		LoggingSystem& operator=(LoggingSystem&& other) = delete;

		virtual void Log(const std::string& message) = 0;
	};
}