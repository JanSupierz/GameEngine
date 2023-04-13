#pragma once
#include "Component.h"
#include "Observer.h"
#include "Subject.h"

namespace dae
{
    class ScoreSystemComponent final: public Component, public Observer, public Subject
    {
    public:
        ScoreSystemComponent() = default;
        virtual ~ScoreSystemComponent() = default;
        ScoreSystemComponent(const ScoreSystemComponent& other) = default;
        ScoreSystemComponent(ScoreSystemComponent&& other) = default;
        ScoreSystemComponent& operator=(const ScoreSystemComponent& other) = default;
        ScoreSystemComponent& operator=(ScoreSystemComponent&& other) = default;

        void Start() const;

        virtual void OnNotify(const void* pdata, const std::string& event) override;
    };
}

