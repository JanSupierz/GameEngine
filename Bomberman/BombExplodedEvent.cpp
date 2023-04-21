#include "BombExplodedEvent.h"
#include "PlayerComponent.h"

dae::BombExplodedEvent::BombExplodedEvent(glm::vec3 position, PlayerComponent* pPlayer)
	: m_pPlayer(pPlayer), m_Position{ position }
{
}

glm::vec3 dae::BombExplodedEvent::GetPosition() const
{
	return m_Position;
}

dae::PlayerComponent* dae::BombExplodedEvent::GetPlayer() const
{
	return m_pPlayer;
}
