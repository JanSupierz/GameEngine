#include "PlayerDiedEvent.h"

namespace dae
{
	void PlayerDiedEvent::operator()(PlayerComponent* pKiller, PlayerComponent* pKilled)
	{
		m_pKiller = pKiller;
		m_pKilled = pKilled;

		Notify(*this);
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