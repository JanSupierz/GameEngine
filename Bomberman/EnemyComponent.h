#pragma once
#include "Component.h"
#include "DeathEvent.h"
#include "Observer.h"

namespace dae
{
	class CollisionEvent;

	class EnemyComponent final : public Component, public Observer<CollisionEvent>
	{
	public:
		EnemyComponent(DeathType enemyType, Subject<CollisionEvent>* pCollider, int priority = 0);
		virtual ~EnemyComponent();
		EnemyComponent(const EnemyComponent& other) = default;
		EnemyComponent(EnemyComponent&& other) = default;
		EnemyComponent& operator=(const EnemyComponent& other) = default;
		EnemyComponent& operator=(EnemyComponent&& other) = default;

		virtual void OnNotify(const CollisionEvent& event) override;
		virtual void OnSubjectDestroy(Subject<CollisionEvent>*) override;

		DeathType GetType() const { return m_EnemyType; };
	private:
		DeathType m_EnemyType;
		Subject<CollisionEvent>* m_pCollider;
	};
}

