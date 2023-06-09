#pragma once
#include "Component.h"
#include "EventListener.h"

namespace dae
{
	class BombExplodedEvent;
	class PlayerComponent;
	class NavigationNode;
	class Scene;
	enum class Direction;

	class BombComponent final : public Component, public EventListener<BombExplodedEvent>
	{
	public:
		BombComponent(float explosionTime, NavigationNode* pNode, PlayerComponent* pPlayer, int range, int priority = 0);
		virtual ~BombComponent();
		BombComponent(const BombComponent& other) = default;
		BombComponent(BombComponent&& other) = default;
		BombComponent& operator=(const BombComponent& other) = default;
		BombComponent& operator=(BombComponent&& other) = default;

		virtual void Update() override;
		void CreateExplosion(Scene* pScene, NavigationNode* pCurrentNode, Direction direction, bool shouldStop) const;
		void OnEvent(const BombExplodedEvent& event) override;
		
		static void SetExplosionSound(const int soundId);
	private:
		void Explode();
		float m_TimeLeft;
		PlayerComponent* m_pPlayer;
		NavigationNode* m_pNode;
		const int m_Range;

		bool m_IsExploded{ false };

		static int s_ExplosionSoundId;
	};
}

