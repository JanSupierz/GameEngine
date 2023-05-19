#include "EnemyComponent.h"
#include "GameObject.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "PlayerComponent.h"
#include "ColliderComponent.h"

using namespace dae;


EnemyComponent::EnemyComponent(DeathType enemyType, Subject<CollisionEvent>* pCollider, int priority)
	:Component{ priority }, m_pCollider{ pCollider }, m_EnemyType{ enemyType }
{
	m_pCollider->AddObserver(this);
}

EnemyComponent::~EnemyComponent()
{
	if (m_pCollider)
	{
		m_pCollider->RemoveObserver(this);
	}
}

void EnemyComponent::EnemyComponent::OnNotify(const CollisionEvent& event)
{
	ColliderComponent* pOther{ event.GetOtherCollider() };

	PlayerComponent* pPlayer{ pOther->GetOwner()->GetComponent<PlayerComponent>().get() };

	if (pPlayer)
	{
		pPlayer->Kill(m_EnemyType);
	}
}

void EnemyComponent::EnemyComponent::OnSubjectDestroy(Subject<CollisionEvent>*)
{
	m_pCollider = nullptr;
}
