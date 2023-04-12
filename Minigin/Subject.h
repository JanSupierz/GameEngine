#pragma once
#include <iostream>
#include <vector>

namespace dae
{
    class Observer;

    class Subject
    {
    public:
        void AddObserver(Observer* pObserver);
        void RemoveObserver(Observer* pObserver);

    protected:
        void Notify(const void* pEntity, const std::string& event) const;

    private:
        std::vector<Observer*> m_pObservers{};
    };
}

