#include "BombExplodedEvent.h"
#include "PlayerComponent.h"
#include "NavigationNode.h"

dae::BombExplodedEvent::BombExplodedEvent(NavigationNode* pNode, PlayerComponent* pPlayer)
	: m_pPlayer(pPlayer), m_pNode{ pNode }
{
}

dae::NavigationNode* dae::BombExplodedEvent::GetNode() const
{
	return m_pNode;
}

dae::PlayerComponent* dae::BombExplodedEvent::GetPlayer() const
{
	return m_pPlayer;
}
