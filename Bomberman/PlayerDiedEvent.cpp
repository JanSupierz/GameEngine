#include "PlayerDiedEvent.h"

namespace dae
{
	PlayerDiedEvent::PlayerDiedEvent(PlayerComponent* pKiller, PlayerComponent* pKilled)
		:m_pKiller{ pKiller }, m_pKilled{ pKilled }
	{
	}

	PlayerComponent* PlayerDiedEvent::GetKiller() const
	{
		return m_pKiller;
	}

	PlayerComponent* PlayerDiedEvent::GetKilled() const
	{
		return m_pKilled;
	}
}