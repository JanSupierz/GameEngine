#pragma once
#include <memory>

template <typename Service>
class NullService;

template<typename Service>
class ServiceLocator final
{
public:
	static Service& GetService() { return *s_Instance; };

	static void RegisterService(std::unique_ptr<Service>&& pService) 
	{ 
		s_Instance = (pService == nullptr ? std::make_unique<NullService<Service>>() : std::move(pService));
	};

private:
	static std::unique_ptr<Service> s_Instance;
};

template <typename Service>
std::unique_ptr<Service> ServiceLocator<Service>::s_Instance = std::make_unique<NullService<Service>>();

