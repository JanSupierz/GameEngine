#include "BombComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "PlayerComponent.h"

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

dae::PlayerComponent* dae::BombComponent::GetPlayer() const
{
	return m_pPlayer.get();
}

void dae::BombComponent::Explode() const
{
	Notify(this, "BombExploded");
	GetOwner()->Destroy();
}
