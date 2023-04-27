#pragma once
#include "Singleton.h"
#include "EventListener.h"

namespace dae
{
    class GainedPointEvent;
    class PlayerDiedEvent;

    class ScoresManager final :public dae::Singleton<ScoresManager>, public EventListener<PlayerDiedEvent>
    {
    public:
        virtual ~ScoresManager();

        virtual void OnEvent(const PlayerDiedEvent& event) override;

        private:
            friend class Singleton<ScoresManager>;
            ScoresManager();
    };
}

