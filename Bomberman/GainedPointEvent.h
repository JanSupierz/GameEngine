#pragma once
#include "QueueEvent.h"
#include <string>

namespace dae
{
	class GainedPointEvent final : public QueueEvent
	{
	public:
		GainedPointEvent(const std::string& name, int score);
		std::string GetName() const;
		int GetScore() const;

	private:
		std::string m_Name;
		int m_Score;
	};
}

