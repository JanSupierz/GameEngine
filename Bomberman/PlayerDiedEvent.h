#pragma once
#include "QueueEvent.h"

namespace dae
{
	class PlayerComponent;

	class PlayerDiedEvent final : public QueueEvent
	{
	public:
		PlayerDiedEvent(PlayerComponent* pKiller, PlayerComponent* pKilled);

		PlayerComponent* GetKiller() const;
		PlayerComponent* GetKilled() const;

	private:
		PlayerComponent* m_pKiller{};
		PlayerComponent* m_pKilled{};
	};
}

