#pragma once
#include <memory>
#include <vector>
#include "EventListener.h"

namespace dae
{
    template<typename EventType>
    class EventQueue final
    {
    public:
        EventQueue()
            :m_Head(0), m_Tail(0), m_Size(5)
        {
            m_pEvents.resize(m_Size);
        }

        ~EventQueue()
        {
            m_pListeners.clear();

            while (!isEmpty())
            {
                m_pEvents[m_Head] = nullptr;
                m_Head = (m_Head + 1) % m_Size;
            }
        }

        void AddListener(EventListener<EventType>* pListener)
        {
            m_pListeners.push_back(pListener);
        }

        void RemoveListener(EventListener<EventType>* pListener)
        {
            m_pListeners.erase(std::remove(m_pListeners.begin(), m_pListeners.end(), pListener), m_pListeners.end());
        }

        void AddEvent(std::unique_ptr<EventType>&& pEvent)
        {
            if (isFull())
            {
                Resize(m_Size * 2);
            }

            m_pEvents[m_Tail] = std::move(pEvent);
            m_Tail = (m_Tail + 1) % m_Size;
        }

        void ProcessEvents()
        {
            while (!isEmpty())
            {
                for (auto pListener : m_pListeners)
                {
                    pListener->OnEvent(*(m_pEvents[m_Head]));
                }

                m_pEvents[m_Head] = nullptr;
                m_Head = (m_Head + 1) % m_Size;
            }
        }

        void Resize(int newSize)
        {
            m_pEvents.resize(newSize);

            const int difference{ m_Head - m_Tail };

            //Head is larger than tail
            //We need to copy the head part to the end
            if (difference > 0)
            {
                const int nrNumbersToCopy{ m_Size - m_Head };

                for (int index{}; index < nrNumbersToCopy; ++index)
                {
                    m_pEvents[newSize - nrNumbersToCopy + index] = std::move(m_pEvents[m_Head + index]);
                    m_pEvents[m_Head + index] = nullptr;
                }

                m_Head = newSize - nrNumbersToCopy;
            }

            m_Size = newSize;
        }

    private:
        bool isFull() const
        { 
            return (m_Tail + 1) % m_Size == m_Head;
        }

        bool isEmpty() const
        {
            return m_Head == m_Tail;
        }

        int m_Size;
        std::vector<EventListener<EventType>*> m_pListeners;
        std::vector<std::unique_ptr<EventType>> m_pEvents;

        int m_Head;
        int m_Tail;
    };
}