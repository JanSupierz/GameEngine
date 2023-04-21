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

         void HandleEvents() const;

	 private:
		 std::unordered_map<std::type_index, EventQueue<QueueEvent>*> m_pQueues{};
	 };
}