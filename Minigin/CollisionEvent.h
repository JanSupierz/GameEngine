#pragma once
#include "Subject.h"

namespace dae
{
	class ColliderComponent;

	class CollisionEvent final: public Subject<CollisionEvent>
	{
	public:
		void NotifyObservers(ColliderComponent* pMyCollider, ColliderComponent* pOtherCollider);
		ColliderComponent* GetMyCollider() const { return m_pMyCollider; };
		ColliderComponent* GetOtherCollider() const { return m_pOtherCollider; };

	private:
		ColliderComponent* m_pMyCollider{ nullptr };
		ColliderComponent* m_pOtherCollider{ nullptr };
	};
}

