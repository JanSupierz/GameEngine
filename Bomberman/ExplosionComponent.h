#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class PlayerComponent;
	class NavigationNode;
	class CollisionEvent;

	class ExplosionComponent final : public Component, public Observer<CollisionEvent>
	{
	public:
		ExplosionComponent(float time, PlayerComponent* pPlayer, NavigationNode* pNode, Subject<CollisionEvent>* pCollider);
		virtual ~ExplosionComponent();
		ExplosionComponent(const ExplosionComponent& other) = default;
		ExplosionComponent(ExplosionComponent&& other) = default;
		ExplosionComponent& operator=(const ExplosionComponent& other) = default;
		ExplosionComponent& operator=(ExplosionComponent&& other) = default;

		virtual void Update() override;
		PlayerComponent* GetPlayer() const { return m_pPlayer; };

		void OnNotify(const CollisionEvent& event) override;
		void OnSubjectDestroy(Subject<CollisionEvent>*) override;
	private:
		float m_TimeLeft;
		NavigationNode* m_pNode;
		PlayerComponent* m_pPlayer;
		Subject<CollisionEvent>* m_pCollider;
	};
}

