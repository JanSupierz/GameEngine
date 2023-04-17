#pragma once
#include <iostream>
#include <vector>
#include <memory>

#include "Observer.h"

namespace dae
{
    //The event type should derive from Subject<EventType>
    template<class EventType>
    class Subject
    {
    public:
        void AddObserver(std::weak_ptr<Observer<EventType>> wpObserver)
        {
            //Check if valid
            if (auto pObserver = wpObserver.lock())
            {
                m_wpObservers.push_back(wpObserver);
            }
        }

        void RemoveObserver(std::weak_ptr<Observer<EventType>> wpObserver)
        {
            auto iterator = std::find_if(m_wpObservers.begin(), m_wpObservers.end(),
                [&](const std::weak_ptr<Observer<EventType>>& wpObs)
                {
                    return wpObs.owner_before(wpObserver) || wpObserver.owner_before(wpObs);
                });

            if (iterator != m_wpObservers.end())
            {
                m_wpObservers.erase(iterator);
            }
        }

    protected:
        void Notify(const EventType& event) const
        {
            for (std::weak_ptr<Observer<EventType>> wpObserver : m_wpObservers)
            {
                //Check if valid
                if (auto pObserver = wpObserver.lock())
                {
                    pObserver->OnNotify(event);
                }
            }
        }

    private:
        std::vector<std::weak_ptr<Observer<EventType>>> m_wpObservers{};
    };
}

