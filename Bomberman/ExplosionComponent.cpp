#include "ExplosionComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "PlayerComponent.h"
#include "EventManager.h"
#include <memory>
#include "BombExplodedEvent.h"

dae::ExplosionComponent::ExplosionComponent(float time, PlayerComponent* pPlayer, NavigationNode* pNode, int priority)
    :Component(priority), m_TimeLeft{ time }, m_pPlayer(pPlayer), m_pNode{ pNode }
{
}

void dae::ExplosionComponent::Update()
{
    m_TimeLeft -= SceneManager::GetInstance().GetDeltaTime();

    EventManager::GetInstance().AddEvent(std::make_shared<dae::BombExplodedEvent>(m_pNode, m_pPlayer));

    if (m_TimeLeft <= 0)
    {
        GetOwner()->Destroy();
    }
}

