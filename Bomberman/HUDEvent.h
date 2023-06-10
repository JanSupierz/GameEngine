#pragma once
#include "QueueEvent.h"
#include <string>

namespace dae
{
	enum class HUDEventType
	{
		Life, Score
	};

	class HUDEvent final : public QueueEvent
	{
	public:
		HUDEvent(HUDEventType type, int index, int value);

		HUDEventType GetType() const;
		int GetIndex() const;
		int GetValue() const;

	private:
		HUDEventType m_Type;
		int m_Index;
		int m_Value;
	};
}

