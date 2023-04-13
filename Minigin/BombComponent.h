#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class GameObject;
	class PlayerComponent;

	class BombComponent final : public Component, public Subject
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

	private:
		void Explode() const;
		float m_TimeLeft;
		std::shared_ptr<PlayerComponent> m_pPlayer;
	};
}

