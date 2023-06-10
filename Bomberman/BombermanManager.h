#pragma once
#include "Singleton.h"
#include "EventListener.h"
#include <vector>
#include <memory>
#include <string>

namespace dae
{
    class DeathEvent;
    
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
        std::string GetLevelPath() const;

        void SetCurrentLevel(int levelIdx);
        int GetCurrentLevelIndex() const;
        void RestartGame();
    private:
        friend class Singleton<BombermanManager>;
        BombermanManager();

        GameMode m_GameMode{ GameMode::SinglePlayer };

        void PlayerDied(int killedPlayerIdx);
        std::vector<std::string> m_LevelPaths;
        std::vector<std::unique_ptr<PlayerInfo>> m_pPlayerInfos;

        int m_CurrentLevel;
    };
}

