#include "DeathEvent.h"

namespace dae
{
	DeathEvent::DeathEvent(DeathType deathType, PlayerComponent* pKiller, PlayerComponent* pKilled)
		:m_pKiller{ pKiller }, m_pKilled{ pKilled }, m_DeathType{ deathType }
	{
	}

	PlayerComponent* DeathEvent::GetKiller() const
	{
		return m_pKiller;
	}

	PlayerComponent* DeathEvent::GetKilled() const
	{
		return m_pKilled;
	}

	DeathType DeathEvent::GetDeathType() const
	{
		return m_DeathType;
	}
}