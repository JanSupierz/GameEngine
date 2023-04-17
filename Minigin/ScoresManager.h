#pragma once
#include "Singleton.h"
#include "Observer.h"

namespace dae
{
    class GainedPointEvent;
    class PlayerDiedEvent;

    class ScoresManager final :public dae::Singleton<ScoresManager>, public Observer<PlayerDiedEvent>
    {
    public:
        ScoresManager();
        virtual ~ScoresManager() = default;
        ScoresManager(const ScoresManager& other) = default;
        ScoresManager(ScoresManager&& other) = default;
        ScoresManager& operator=(const ScoresManager& other) = default;
        ScoresManager& operator=(ScoresManager&& other) = default;

        virtual void OnNotify(const PlayerDiedEvent& event) override;

        GainedPointEvent* GetGainedPointEvent() const;

    private:
        std::unique_ptr<GainedPointEvent> m_pGainedPoint;
    };
}

