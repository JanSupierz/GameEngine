#pragma once
#include "EventQueue.h"
#include "QueueEvent.h"
#include "Singleton.h"
#include <typeindex>
#include <unordered_map>

namespace dae
{
	 class EventManager final: public Singleton<EventManager>
	 {
	 public:
         ~EventManager()
         {
             for (auto pQueue : m_pQueues)
             {
                 delete pQueue.second;
             }
         }

         template<class EventType>
         void AddEvent(std::shared_ptr<EventType> pEvent)
         {
             GetQueue<EventType>()->AddEvent(pEvent);
             m_IsDirty = true;
         }

         template<class EventType>
         void AddListener(EventListener<EventType>* pListener)
         {
             GetQueue<EventType>()->AddListener(pListener);
         }

         template<class EventType>
         void RemoveListener(EventListener<EventType>* pListener)
         {
             GetQueue<EventType>()->RemoveListener(pListener);
         }

         void HandleEvents();

	 private:
         template<class EventType>
         EventQueue<EventType>* GetQueue()
         {
             auto found = m_pQueues.find(typeid(EventQueue<EventType>));
             if (found != m_pQueues.end())
             {
                 return reinterpret_cast<EventQueue<EventType>*>(found->second);
             }
             else
             {
                 static_assert(std::is_base_of<QueueEvent, EventType>::value, "EventType must be derived from QueueEvent");

                 auto pQueue{ new EventQueue<EventType>() };
                 m_pQueues[typeid(EventQueue<EventType>)] = reinterpret_cast<EventQueue<QueueEvent>*>(pQueue);

                 return pQueue;
             }
         }

		 std::unordered_map<std::type_index, EventQueue<QueueEvent>*> m_pQueues{};
         bool m_IsDirty{ false };
	 };
}