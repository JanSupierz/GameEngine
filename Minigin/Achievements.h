#pragma once
#include "Singleton.h"
#include "Observer.h"

namespace dae
{
    class Achievements : public Singleton<Achievements>, public Observer
    {
    public:
        virtual void OnNotify(const void* pEntity, const std::string& message) override;

    private:
        friend class Singleton<Achievements>;
        Achievements() = default;

        int m_Counter{};
    };
}

