#pragma once
#include "Observer.h"
#include "Subject.h"

namespace dae
{
    class ScoresManager final : public Observer, public Subject
    {
    public:
        ScoresManager() = default;
        virtual ~ScoresManager() = default;
        ScoresManager(const ScoresManager& other) = default;
        ScoresManager(ScoresManager&& other) = default;
        ScoresManager& operator=(const ScoresManager& other) = default;
        ScoresManager& operator=(ScoresManager&& other) = default;

        virtual void OnNotify(const void* pdata, const std::string& event) override;
    };
}

