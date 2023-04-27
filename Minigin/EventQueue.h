#pragma once
#include <memory>
#include <vector>
#include "EventListener.h"
#include <iostream>
namespace dae
{
    template<typename EventType>
    class EventQueue
    {
    public:
        EventQueue()
            :m_Head(0), m_Tail(0), m_Size(5)
        {
            m_pEvents.resize(m_Size);
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
                Resize(m_Size * 2);
            }

            m_pEvents[m_Head] = pEvent;
            m_Head = (m_Head + 1) % m_Size;
        }

        void ProcessEvents()
        {
            while (!isEmpty())
            {
                for (auto pListener : m_pListeners)
                {
                    pListener->OnEvent(*(m_pEvents[m_Tail]));
                }

                m_Tail = (m_Tail + 1) % m_Size;
            }
        }

        void Resize(int newSize)
        {
            m_pEvents.resize(newSize);
            m_Size = newSize;
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

        int m_Size;
        std::vector<EventListener<EventType>*> m_pListeners;
        std::vector<std::shared_ptr<EventType>> m_pEvents;

        int m_Head;
        int m_Tail;
    };
}