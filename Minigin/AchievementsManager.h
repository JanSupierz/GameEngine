#pragma once
#include "Observer.h"

namespace dae
{
    class AchievementsManager final : public Observer
    {
    public:
        AchievementsManager() = default;
        virtual ~AchievementsManager() = default;
        AchievementsManager(const AchievementsManager& other) = default;
        AchievementsManager(AchievementsManager&& other) = default;
        AchievementsManager& operator=(const AchievementsManager& other) = default;
        AchievementsManager& operator=(AchievementsManager&& other) = default;

        virtual void OnNotify(const void* pdata, const std::string& event) override;
    };
}

