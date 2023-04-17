#include "ScoresManager.h"
#include "PlayerComponent.h"
#include "GainedPointEvent.h"
#include "PlayerDiedEvent.h"

dae::ScoresManager::ScoresManager()
	:m_pGainedPoint{ std::make_unique<GainedPointEvent>() }
{
}

void dae::ScoresManager::OnNotify(const PlayerDiedEvent& event)
{
	const auto pKiller{ event.GetKiller() };
	const auto pKilled{ event.GetKilled() };

	if (pKiller != nullptr && pKilled != nullptr && pKiller != pKilled)
	{
		pKiller->SetScore(pKiller->GetScore() + 50);
		m_pGainedPoint->operator()(pKiller);
	}
}

dae::GainedPointEvent* dae::ScoresManager::GetGainedPointEvent() const
{
	return m_pGainedPoint.get();
}
