#pragma once
#include "Singleton.h"
#include "EventListener.h"
#include <unordered_map>
#include <string>

namespace dae
{
    class GainedPointEvent;
    class DeathEvent;
    
    enum class GameMode
    {
        Versus, Coop, SinglePlayer
    };

    class BombermanManager final :public dae::Singleton<BombermanManager>, public EventListener<DeathEvent>
    {
    public:
        virtual ~BombermanManager();
        virtual void OnEvent(const DeathEvent& event) override;

    private:
        friend class Singleton<BombermanManager>;
        BombermanManager();

        GameMode m_ScoreMode{ GameMode::SinglePlayer };

        std::unordered_map<std::string, int> m_Scores{};
    };
}

