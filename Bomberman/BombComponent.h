#pragma once
#include "Component.h"

namespace dae
{
	class PlayerComponent;
	class NavigationNode;
	class Scene;

	class BombComponent final : public Component
	{
	public:
		BombComponent(float explosionTime, NavigationNode* pNode, PlayerComponent* pPlayer, int range, int priority = 0);
		virtual ~BombComponent() = default;
		BombComponent(const BombComponent& other) = default;
		BombComponent(BombComponent&& other) = default;
		BombComponent& operator=(const BombComponent& other) = default;
		BombComponent& operator=(BombComponent&& other) = default;

		virtual void Update() override;
		void CreateExplosion(Scene* pScene, NavigationNode* pCurrentNode) const;
	private:
		void Explode();
		float m_TimeLeft;
		PlayerComponent* m_pPlayer;
		NavigationNode* m_pNode;
		const int m_Range;
	};
}

