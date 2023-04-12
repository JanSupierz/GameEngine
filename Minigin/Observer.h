#pragma once
#include <iostream>

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const void* pEntity, const std::string& event) = 0;
	};
}

