#include "BombermanManager.h"
#include "PlayerComponent.h"
#include "HUDEvent.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "Logger.h"
#include "PowerUp.h"

dae::BombermanManager::BombermanManager()
{
	EventManager::GetInstance().AddListener(this);
}

void dae::BombermanManager::SetGameMode(GameMode gameMode)
{
	m_GameMode = gameMode;
}

dae::GameMode dae::BombermanManager::GetMode() const
{
	return m_GameMode;
}

int dae::BombermanManager::GetExplosionRange(int index) const
{
	return m_pPlayerInfos[index]->explosionRange;
}

void dae::BombermanManager::PlayerDied(int killedPlayerIdx)
{
	auto& pPlayerInfo{ m_pPlayerInfos[killedPlayerIdx] };
	EventManager::GetInstance().AddEvent(std::make_shared<HUDEvent>(HUDEventType::Life, killedPlayerIdx, --pPlayerInfo->nrLives));
}

bool dae::BombermanManager::CanDetonate(int index) const
{
	return m_pPlayerInfos[index]->canDetonate;
}

dae::BombermanManager::~BombermanManager()
{
}

void dae::BombermanManager::Clear()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::BombermanManager::RefreshHUD()
{
	for (auto& pPlayer : m_pPlayerInfos)
	{
		EventManager::GetInstance().AddEvent(std::make_shared<HUDEvent>(HUDEventType::Life, pPlayer->index, pPlayer->nrLives));
		EventManager::GetInstance().AddEvent(std::make_shared<HUDEvent>(HUDEventType::Score, pPlayer->index, pPlayer->score));
	}
}

void dae::BombermanManager::OnEvent(const DeathEvent& event)
{
	const auto pKiller{ event.GetKiller() };
	const auto pKilled{ event.GetKilled() };

	//Enemy killed player -> no score
	if (pKiller == nullptr)
	{
		PlayerDied(pKilled->GetIndex());
	}
	else
	{
		auto& pPlayerInfo{ m_pPlayerInfos[pKiller->GetIndex()] };

		switch (event.GetDeathType())
		{
		case DeathType::Balloom:
		{
			pPlayerInfo->score += 100;
		}
		break;
		case DeathType::Oneal:
		{
			pPlayerInfo->score += 200;
		}
		break;
		case DeathType::Doll:
		{
			pPlayerInfo->score += 400;
		}
		break;
		case DeathType::Minvo:
		{
			pPlayerInfo->score += 800;
		}
		break;

		//Player killed player
		case DeathType::Player:
		{
			if (m_GameMode == GameMode::Versus && pKilled != pKiller)
			{
				pPlayerInfo->score += 1000;
			}

			PlayerDied(pKilled->GetIndex());
		}
		break;

		default:
			break;
		}

		EventManager::GetInstance().AddEvent(std::make_shared<HUDEvent>(HUDEventType::Score, pPlayerInfo->index, pPlayerInfo->score));
	}
}

void dae::BombermanManager::AddPlayer(const std::string& name)
{
	m_pPlayerInfos.push_back(std::make_unique<PlayerInfo>(name, static_cast<int>(m_pPlayerInfos.size())));
}

void dae::BombermanManager::AddPowerUp(dae::PowerUpType type, int index)
{
	if (type == PowerUpType::Flames)
	{
		if(m_pPlayerInfos[index]->explosionRange < 6)
		{
			++m_pPlayerInfos[index]->explosionRange;
		}
	}
	else if (type == PowerUpType::Detonator)
	{
		m_pPlayerInfos[index]->canDetonate = true;
	}
	else if (type == PowerUpType::ExtraBomb)
	{
		++m_pPlayerInfos[index]->maxNrBombs;
	}
}
