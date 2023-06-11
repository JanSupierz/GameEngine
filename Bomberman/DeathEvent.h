#pragma once
#include "QueueEvent.h"

namespace dae
{
	class PlayerComponent;

	enum class DeathType
	{
		Player,
		BalloomPlayer,
		Balloom,
		Oneal,
		Doll,
		Minvo
	};

	class DeathEvent final : public QueueEvent
	{
	public:
		DeathEvent(DeathType deathType, PlayerComponent* pKiller, PlayerComponent* pKilled = nullptr);

		PlayerComponent* GetKiller() const;
		PlayerComponent* GetKilled() const;
		DeathType GetDeathType() const;

	private:
		PlayerComponent* m_pKiller{};
		PlayerComponent* m_pKilled{};
		DeathType m_DeathType{};
	};
}

