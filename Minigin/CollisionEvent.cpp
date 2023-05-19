#include "CollisionEvent.h"

void dae::CollisionEvent::NotifyObservers(ColliderComponent* pMyCollider, ColliderComponent* pOtherCollider)
{
	m_pMyCollider = pMyCollider;
	m_pOtherCollider = pOtherCollider;

	Notify(*this);
}
