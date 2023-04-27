#include "BombComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "PlayerComponent.h"
#include "EventQueue.h"
#include "BombExplodedEvent.h"
#include "EventManager.h"

dae::BombComponent::BombComponent(float time, std::shared_ptr<PlayerComponent> pPlayer, int priority)
	:Component(priority), m_TimeLeft{ time }, m_pPlayer(pPlayer)
{
}

void dae::BombComponent::Update()
{
	m_TimeLeft -= SceneManager::GetInstance().GetDeltaTime();

	if (m_TimeLeft <= 0)
	{
		Explode();
	}
}

void dae::BombComponent::Explode()
{
	dae::EventManager::GetInstance().AddEvent(std::make_shared<BombExplodedEvent>(GetOwner()->GetWorldPosition(), m_pPlayer.get()));
	GetOwner()->Destroy();
}

