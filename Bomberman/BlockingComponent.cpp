#include "BlockingComponent.h"
#include "GameObject.h"
#include "NavigationNode.h"
#include "EventManager.h"
#include "BombExplodedEvent.h"

dae::BlockingComponent::BlockingComponent(NavigationNode* pNode, int priority)
	:Component(priority), m_pNode{ pNode }
{
	EventManager::GetInstance().AddListener(this);
	pNode->IsBlocked(true);
}

dae::BlockingComponent::~BlockingComponent()
{
	EventManager::GetInstance().RemoveListener(this);
	m_pNode->IsBlocked(false);
}

void dae::BlockingComponent::OnEvent(const BombExplodedEvent& event)
{
	if (event.GetNode() == m_pNode)
	{
		GetOwner()->Destroy();
	}
}

