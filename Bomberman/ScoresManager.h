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
        ScoresManager();
        virtual ~ScoresManager();
        ScoresManager(const ScoresManager& other) = default;
        ScoresManager(ScoresManager&& other) = default;
        ScoresManager& operator=(const ScoresManager& other) = default;
        ScoresManager& operator=(ScoresManager&& other) = default;

        virtual void OnEvent(const PlayerDiedEvent& event) override;
    };
}

