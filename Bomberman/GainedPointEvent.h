#pragma once
#include "QueueEvent.h"

namespace dae
{
	class PlayerComponent;

	class GainedPointEvent final : public QueueEvent
	{
	public:
		GainedPointEvent(PlayerComponent* pPlayer);
		PlayerComponent* GetPlayer() const;
	private:
		PlayerComponent* m_pPlayer;
	};
}

