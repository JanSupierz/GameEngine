#include "ScoresManager.h"
#include "PlayerComponent.h"

void dae::ScoresManager::OnNotify(const void* pData, const std::string& event)
{
	if (event == "PlayerDied")
	{
		const auto pPlayerPair{ static_cast<const std::pair<PlayerComponent*, PlayerComponent*>*>(pData) };

		const auto pKiller{ pPlayerPair->first };
		const auto pKilled{ pPlayerPair->second };

		if (pKiller && pKilled && pKiller != pKilled)
		{
			Notify(pKiller, "GainedPoint");
		}

		return;
	}
}