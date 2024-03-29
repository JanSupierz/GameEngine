#pragma once
#include "Singleton.h"
#include "EventListener.h"
#include <vector>
#include <memory>
#include <string>

namespace dae
{
    class DeathEvent;
    class GameObject;

    enum class PowerUpType;

    enum class GameMode
    {
        Versus, Coop, SinglePlayer
    };

    struct PlayerInfo
    {
        std::string name;

        int index{ 0 };
        int nrLives{ 4 };
        int score{ 0 };
        int explosionRange{ 1 };
        int maxNrBombs{ 1 };
        bool canDetonate{ false };
    };

    class BombermanManager final :public dae::Singleton<BombermanManager>, public EventListener<DeathEvent>
    {
    public:
        virtual ~BombermanManager();
        virtual void OnEvent(const DeathEvent& event) override;

        void AddPlayer(const std::string& name);
        void AddPowerUp(PowerUpType type, int index);
        void RefreshHUD();

        void SetGameMode(GameMode gameMode);
        GameMode GetMode() const;

        void NextLevel();
        bool CanDetonate(int index) const;
        int GetExplosionRange(int index) const;
        int GetMaxNrBombs(int index) const;
        void AddLevelPath(const std::string& levelPath);
        void SetSound(int doorSoundId);
        std::string GetLevelPath() const;

        void SetCurrentLevel(int levelIdx);
        int GetCurrentLevelIndex() const;
        void RestartGame();

        void AddEnemy();
        void RemoveEnemy();
        int GetNrEnemies() const;

        void AddScore(const std::string& name, int score);
        std::vector<std::pair<std::string, int>>  GetTopScores(int maxNrScores);

        void SetPlayerObject(GameObject* pObject);
        std::vector<GameObject*> GetPlayerObjects() const;

        void ClearPlayerInfos();
    private:
        friend class Singleton<BombermanManager>;
        BombermanManager();

        GameMode m_GameMode{ GameMode::SinglePlayer };

        void PlayerDied(int killedPlayerIdx);
        std::vector<std::string> m_LevelPaths;
        std::vector<std::unique_ptr<PlayerInfo>> m_pPlayerInfos;

        int m_CurrentLevel;
        int m_NrEnemies;

        std::vector<std::pair<std::string, int>> m_Scores;
        std::vector<GameObject*> m_pPlayerObjects;
    };
}

