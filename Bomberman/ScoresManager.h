#pragma once
#include "Singleton.h"
#include "EventListener.h"

namespace dae
{
    class GainedPointEvent;
    class DeathEvent;
    
    enum class GameMode
    {
        Versus, Coop, SinglePlayer
    };

    class ScoresManager final :public dae::Singleton<ScoresManager>, public EventListener<DeathEvent>
    {
    public:
        virtual ~ScoresManager();

        virtual void OnEvent(const DeathEvent& event) override;

        private:
            friend class Singleton<ScoresManager>;
            ScoresManager();

            GameMode m_ScoreMode{ GameMode::SinglePlayer };
    };
}

