#pragma once
#include <memory>

namespace dae
{
	template <typename T>
	class Singleton
	{
	public:
		static std::shared_ptr<T>& GetInstance()
		{
			static std::shared_ptr<T> instance = std::shared_ptr<T>(new T());
			return instance;
		}

		virtual ~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	protected:
		Singleton() = default;
	};
}