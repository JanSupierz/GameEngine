#include "ScoreSystemComponent.h"
#include "PlayerComponent.h"

void dae::ScoreSystemComponent::OnNotify(const void* pData, const std::string& event)
{
	if (event == "PlayerDied")
	{
		const auto pPlayerPair{ static_cast<const std::pair<PlayerComponent*, PlayerComponent*>*>(pData) };

		const auto pKiller{ pPlayerPair->first };
		const auto pKilled{ pPlayerPair->second };

		if (pKiller != pKilled)
		{
			Notify(pKiller, "GainedPoint");
		}

		return;
	}
}

void dae::ScoreSystemComponent::Start() const
{
	Notify(nullptr, "Start");
}
