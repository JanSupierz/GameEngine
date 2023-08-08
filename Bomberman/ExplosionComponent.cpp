#include "ExplosionComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "ExplosionComponent.h"
#include <memory>
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

dae::ExplosionComponent::ExplosionComponent(float time, PlayerComponent* pPlayer, NavigationNode* pNode, Subject<CollisionEvent>* pCollider)
    :Component(-8), m_TimeLeft{ time }, m_pPlayer(pPlayer), m_pNode{ pNode }, m_pCollider{ pCollider }
{
    m_pCollider->AddObserver(this);
}

dae::ExplosionComponent::~ExplosionComponent()
{
    if (m_pCollider)
    {
        m_pCollider->RemoveObserver(this);
    }
}

void dae::ExplosionComponent::Update()
{
    m_TimeLeft -= SceneManager::GetInstance().GetDeltaTime();

    if (m_TimeLeft <= 0)
    {
        GetOwner()->Destroy();
    }
}

void dae::ExplosionComponent::OnNotify(const CollisionEvent& event)
{
    ColliderComponent* pOther{ event.GetOtherCollider() };

    if (pOther)
    {
        auto pPlayer{ pOther->GetOwner()->GetComponent<PlayerComponent>() };

        if (pPlayer)
        {
            pPlayer->Kill(DeathType::Player, m_pPlayer);
            return;
        }

        auto pEnemy{ pOther->GetOwner()->GetComponent<EnemyComponent>() };

        if(pEnemy)
        {
            pEnemy->GetOwner()->Destroy();

            EventManager::GetInstance().AddEvent(std::make_unique<DeathEvent>(pEnemy->GetType(), m_pPlayer));
            return;
        }
    }
}

void dae::ExplosionComponent::OnSubjectDestroy(Subject<CollisionEvent>*)
{
    m_pCollider = nullptr;
}

