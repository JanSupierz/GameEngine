#include "BombExplodedEvent.h"
#include "PlayerComponent.h"

dae::BombExplodedEvent::BombExplodedEvent(const glm::vec2& position, PlayerComponent* pPlayer)
	: m_pPlayer(pPlayer), m_Position{ position }
{
}

glm::vec2 dae::BombExplodedEvent::GetPosition() const
{
	return m_Position;
}

dae::PlayerComponent* dae::BombExplodedEvent::GetPlayer() const
{
	return m_pPlayer;
}
