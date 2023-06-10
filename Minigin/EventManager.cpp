#include "EventManager.h"

void dae::EventManager::HandleEvents()
{
	if (m_IsDirty)
	{
		if (!m_pNewQueues.empty())
		{
			for (const auto& pQueue : m_pNewQueues)
			{
				m_pQueues[pQueue.first] = pQueue.second;
			}

			m_pNewQueues.clear();
		}

		for (const auto& pQueue : m_pQueues)
		{
			pQueue.second->ProcessEvents();
		}

		m_IsDirty = false;
	}
}

void dae::EventManager::Clear()
{
	for (auto pQueue : m_pQueues)
	{
		delete pQueue.second;
	}

	for (auto pQueue : m_pNewQueues)
	{
		delete pQueue.second;
	}
}
