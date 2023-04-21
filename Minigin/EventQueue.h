#pragma once
#include <memory>
#include <vector>
#include "EventListener.h"

namespace dae
{
    template<typename EventType>
    class EventQueue
    {
    public:
        EventQueue() 
            : m_pEvents{}, m_Head(0), m_Tail(0)
        {
        }

        void AddListener(EventListener<EventType>* pListener) 
        {
            m_pListeners.push_back(pListener);
        }

        void RemoveListener(EventListener<EventType>* pListener)
        {
            m_pListeners.erase(std::remove(m_pListeners.begin(), m_pListeners.end(), pListener), m_pListeners.end());
        }

        void AddEvent(std::shared_ptr<EventType> pEvent)
        {
            if (isFull()) 
            {
                //Replace oldest event
                m_Tail = (m_Tail + 1) % m_Size; 
            }

            m_pEvents[m_Head] = pEvent;
            m_Head = (m_Head + 1) % m_Size;
        }

        void ProcessEvents() 
        {
            while (!isEmpty()) 
            {
                std::shared_ptr<EventType> pEvent{ m_pEvents[m_Tail] };

                for (auto pListener : m_pListeners) 
                {
                    pListener->OnEvent(*pEvent);
                }

                m_Tail = (m_Tail + 1) % m_Size;
            }
        }

    private:
        bool isFull() const 
        {
            return (m_Head + 1) % m_Size == m_Tail;
        }

        bool isEmpty() const 
        {
            return m_Head == m_Tail;
        }

        static const int m_Size{20};
        std::vector<EventListener<EventType>*> m_pListeners;
        std::shared_ptr<EventType> m_pEvents[m_Size];

        int m_Head;
        int m_Tail;
    };
}

