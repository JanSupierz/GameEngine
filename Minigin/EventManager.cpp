#include "EventManager.h"

void dae::EventManager::HandleEvents() const
{
	for (const auto& pQueue : m_pQueues)
	{
		pQueue.second->ProcessEvents();
	}
}