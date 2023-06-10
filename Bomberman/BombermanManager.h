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

        void Clear();

        void AddPlayer(const std::string& name);
        void AddPowerUp(PowerUpType type, int index);
        void RefreshHUD();
        void SetGameMode(GameMode gameMode);
        GameMode GetMode() const;
        bool CanDetonate(int index) const;
        int GetExplosionRange(int index) const;
    private:
        friend class Singleton<BombermanManager>;
        BombermanManager();

        GameMode m_GameMode{ GameMode::SinglePlayer };

        void PlayerDied(int killedPlayerIdx);

        std::vector<std::unique_ptr<PlayerInfo>> m_pPlayerInfos;
    };
}

