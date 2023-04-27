#include "EventManager.h"

void dae::EventManager::HandleEvents()
{
	if (m_IsDirty)
	{
		for (const auto& pQueue : m_pQueues)
		{
			pQueue.second->ProcessEvents();
		}

		m_IsDirty = false;
	}
}
