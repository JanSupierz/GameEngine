#include "BombComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "PlayerComponent.h"

#include "BombExplodedEvent.h"

dae::BombComponent::BombComponent(float time, std::shared_ptr<PlayerComponent> pPlayer, int priority)
	:Component(priority), m_TimeLeft{ time }, m_pPlayer(pPlayer), m_pBombExploded{std::make_unique<BombExplodedEvent>() }
{
}

void dae::BombComponent::Update()
{
	m_TimeLeft -= SceneManager::GetInstance()->GetDeltaTime();

	if (m_TimeLeft <= 0)
	{
		Explode();
	}
}

void dae::BombComponent::Explode()
{
	m_pBombExploded->operator()(this);
	GetOwner()->Destroy();
}

dae::PlayerComponent* dae::BombComponent::GetPlayer() const
{
	return m_pPlayer.get();
}

dae::BombExplodedEvent* dae::BombComponent::GetExplodeEvent() const
{
	return m_pBombExploded.get();
}
