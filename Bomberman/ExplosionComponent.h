#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class PlayerComponent;
	class BombExplodedEvent;
	class NavigationNode;

	class ExplosionComponent final : public Component
	{
	public:
		ExplosionComponent(float time, PlayerComponent* pPlayer, NavigationNode* pNode, int priority = 0);
		virtual ~ExplosionComponent() = default;
		ExplosionComponent(const ExplosionComponent& other) = default;
		ExplosionComponent(ExplosionComponent&& other) = default;
		ExplosionComponent& operator=(const ExplosionComponent& other) = default;
		ExplosionComponent& operator=(ExplosionComponent&& other) = default;

		virtual void Update() override;
	private:
		float m_TimeLeft;
		NavigationNode* m_pNode;
		PlayerComponent* m_pPlayer;
	};
}

