#pragma once
#pragma once
#include "LoggingSystem.h"
#include <memory>

namespace dae
{
	class Logger final
	{
		class NullLoggingSystem final: public LoggingSystem
		{
		public:
		#if _DEBUG
			virtual void Log(const std::string& message) { std::cout << "Log: " << message << '\n'; };
		#else
			virtual void Log(const std::string&) {};
		#endif
		};

	public:
		static LoggingSystem& Get();
		static void RegisterService(std::unique_ptr<LoggingSystem>&& pService);

	private:
		static std::unique_ptr<LoggingSystem> s_Instance;
	};
};

