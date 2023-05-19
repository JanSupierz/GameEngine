#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include "Observer.h"

namespace dae
{
    //The event type should derrive from Subject<EventType> and wrap the Notify function
    template<class EventType>
    class Subject
    {
    public:
        virtual ~Subject()
        {
            for (const auto& pObserver : m_pObservers)
            {
                pObserver->OnSubjectDestroy(this);
            }
        }

        void AddObserver(Observer<EventType>* pObserver)
        {
            m_pObservers.push_back(pObserver);
        }

        void RemoveObserver(Observer<EventType>* pObserver)
        {
            m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver), m_pObservers.end());
        }

    protected:
        void Notify(const EventType& event) const
        {
            for (const auto& pObserver : m_pObservers)
            {
                pObserver->OnNotify(event);
            }
        }

    private:
        std::vector<Observer<EventType>*> m_pObservers{};
    };
}

