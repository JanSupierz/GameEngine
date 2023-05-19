#include "ScoresManager.h"
#include "PlayerComponent.h"
#include "GainedPointEvent.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "Logger.h"

dae::ScoresManager::ScoresManager()
{
	EventManager::GetInstance().AddListener(this);
}

dae::ScoresManager::~ScoresManager()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::ScoresManager::OnEvent(const DeathEvent& event)
{
	const auto pKiller{ event.GetKiller() };
	const auto pKilled{ event.GetKilled() };

	//Enemy killed player -> no score
	if (pKiller == nullptr) return;

	//Player killed enemy or player
	int gainedScore{ 0 };

	switch (event.GetDeathType())
	{
		case DeathType::Balloom:
		{
			Logger::Get().Log("Balloom killed by " + pKiller->GetName());
			gainedScore = 100;
		}
		break;
		case DeathType::Oneal:
		{
			Logger::Get().Log("Oneal killed by " + pKiller->GetName());
			gainedScore = 200;
		}
		break;
		case DeathType::Doll:
		{
			Logger::Get().Log("Doll killed by " + pKiller->GetName());
			gainedScore = 400;
		}
		break;
		case DeathType::Minvo:
		{
			Logger::Get().Log("Minvo killed by " + pKiller->GetName());
			gainedScore = 800;
		}
		break;

		//Player killed player
		case DeathType::Player:
		{
			Logger::Get().Log(pKilled->GetName() + " killed by " + pKiller->GetName());

			switch (m_ScoreMode)
			{
				case dae::GameMode::Versus:
				{
					if (pKilled != pKiller)
					{
						gainedScore = 100;
					}
				}
				break;
				case dae::GameMode::Coop:
				default:
				{
					gainedScore = -100;
				}
				break;
			}
		}
		break;

		default:
			break;
	}

	pKiller->SetScore(pKiller->GetScore() + gainedScore);
	EventManager::GetInstance().AddEvent(std::make_shared<GainedPointEvent>(pKiller));
}