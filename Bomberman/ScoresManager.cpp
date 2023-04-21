#include "ScoresManager.h"
#include "PlayerComponent.h"
#include "GainedPointEvent.h"
#include "PlayerDiedEvent.h"
#include "EventManager.h"

dae::ScoresManager::ScoresManager()
{
	EventManager::GetInstance().GetQueue<PlayerDiedEvent>()->AddListener(this);
}

dae::ScoresManager::~ScoresManager()
{
	EventManager::GetInstance().GetQueue<PlayerDiedEvent>()->RemoveListener(this);
}

void dae::ScoresManager::OnEvent(const PlayerDiedEvent& event)
{
	const auto pKiller{ event.GetKiller() };
	const auto pKilled{ event.GetKilled() };

	if (pKiller != nullptr && pKilled != nullptr && pKiller != pKilled)
	{
		pKiller->SetScore(pKiller->GetScore() + 50);
		EventManager::GetInstance().GetQueue<GainedPointEvent>()->AddEvent(std::make_shared<GainedPointEvent>(pKiller));
	}
}
