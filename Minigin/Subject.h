#pragma once
#include <iostream>
#include <vector>
#include <memory>

namespace dae
{
    class Observer;

    class Subject
    {
    public:
        virtual ~Subject();
        void AddObserver(std::weak_ptr<Observer>  wpObserver);
        void RemoveObserver(std::weak_ptr<Observer>  wpObserver);

    protected:
        void Notify(const void* pData, const std::string& event) const;

    private:
        std::vector<std::weak_ptr<Observer>> m_wpObservers{};
    };
}

