#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class GameObject;
	class PlayerComponent;
	class BombExplodedEvent;

	class BombComponent final : public Component
	{
	public:
		BombComponent(float explosionTime, std::shared_ptr<PlayerComponent> pPlayer, int priority = 0);
		virtual ~BombComponent() = default;
		BombComponent(const BombComponent& other) = default;
		BombComponent(BombComponent&& other) = default;
		BombComponent& operator=(const BombComponent& other) = default;
		BombComponent& operator=(BombComponent&& other) = default;

		virtual void Update() override;
		PlayerComponent* GetPlayer() const;
		BombExplodedEvent* GetExplodeEvent() const;
	private:
		void Explode();
		float m_TimeLeft;
		std::shared_ptr<PlayerComponent> m_pPlayer;

		std::shared_ptr<BombExplodedEvent> m_pBombExploded;
	};
}

