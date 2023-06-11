#include "BombermanManager.h"
#include "PlayerComponent.h"
#include "HUDEvent.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "Logger.h"
#include "PowerUp.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

dae::BombermanManager::BombermanManager()
	:m_CurrentLevel{ 0 }, m_pPlayerObjects{}, m_NrEnemies{}
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

	if (pPlayerInfo->nrLives <= 0)
	{
		RestartGame();
	}

	SceneManager::GetInstance().GetScene("LoadingScene")->Load();

	m_pPlayerObjects.clear();
}

bool dae::BombermanManager::CanDetonate(int index) const
{
	return m_pPlayerInfos[index]->canDetonate;
}

void dae::BombermanManager::NextLevel()
{
	++m_CurrentLevel;

	m_pPlayerObjects.clear();

	if (m_CurrentLevel >= static_cast<int>(m_LevelPaths.size()))
	{
		RestartGame();
		SceneManager::GetInstance().GetScene("ScoreScene")->Load();
	}
	else
	{
		SceneManager::GetInstance().GetScene("LoadingScene")->Load();
	}
}

int dae::BombermanManager::GetMaxNrBombs(int index) const
{
	return m_pPlayerInfos[index]->maxNrBombs;
}

std::string dae::BombermanManager::GetLevelPath() const
{
	return m_LevelPaths[m_CurrentLevel];
}

void dae::BombermanManager::SetCurrentLevel(int levelIdx)
{
	m_CurrentLevel = levelIdx;
}

int dae::BombermanManager::GetCurrentLevelIndex() const
{
	return m_CurrentLevel;
}

void dae::BombermanManager::RestartGame()
{
	for(const auto& pPlayer: m_pPlayerInfos)
	{
		AddScore(pPlayer->name, pPlayer->score);

		pPlayer->canDetonate = false;
		pPlayer->explosionRange = 1;
		pPlayer->maxNrBombs = 1;
		pPlayer->nrLives = 4;
		pPlayer->score = 0;
	}

	m_CurrentLevel = 0;
}

void dae::BombermanManager::AddEnemy()
{
	++m_NrEnemies;
}

void dae::BombermanManager::RemoveEnemy()
{
	--m_NrEnemies;
}

void dae::BombermanManager::AddScore(const std::string& name, int score)
{
	auto found{ std::find_if(m_Scores.begin(), m_Scores.end(), [&](const auto& p1)
		{return p1.first == name && p1.second == score; }) };

	if (found == m_Scores.end())
	{
		m_Scores.emplace_back(std::make_pair(name, score));
	}
}

std::vector<std::pair<std::string, int>> dae::BombermanManager::GetTopScores(int maxNrScores)
{
	std::sort(m_Scores.begin(), m_Scores.end(), [](auto p1, auto p2) {return p1.second > p2.second; });

	std::vector<std::pair<std::string, int>> scores{};
	for (int index{ 0 }; index < std::min(static_cast<int>(m_Scores.size()), maxNrScores); ++index)
	{
		scores.emplace_back(m_Scores[index]);
	}

	return scores;
}

void dae::BombermanManager::SetPlayerObject(dae::GameObject* pObject)
{
	m_pPlayerObjects.emplace_back(pObject);
}

std::vector<dae::GameObject*> dae::BombermanManager::GetPlayerObjects() const
{
	return m_pPlayerObjects;
}

void dae::BombermanManager::ClearPlayerInfos()
{
	m_pPlayerInfos.clear();
	m_pPlayerObjects.clear();
}

int dae::BombermanManager::GetNrEnemies() const
{
	return m_NrEnemies;
}

void dae::BombermanManager::AddLevelPath(const std::string& levelPath)
{
	m_LevelPaths.emplace_back(levelPath);
}

dae::BombermanManager::~BombermanManager()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::BombermanManager::RefreshHUD()
{
	for (const auto& pPlayer : m_pPlayerInfos)
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
		if (event.GetDeathType() == DeathType::BalloomPlayer)
		{
			m_pPlayerInfos[1]->score += 1000;
		}

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
		if(m_pPlayerInfos[index]->explosionRange < 4)
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
