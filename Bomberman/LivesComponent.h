#pragma once
#include "Component.h"
#include "EventListener.h"
#include <string>
#include <memory>

namespace dae
{
	class GameObject;
	class TextComponent;
	class PlayerComponent;
	class DeathEvent;

	class LivesComponent final : public Component, public EventListener<DeathEvent>
	{
	public:
		LivesComponent(PlayerComponent* pPlayer, std::shared_ptr<TextComponent> pTextComponent = nullptr, int priority = 0);
		virtual ~LivesComponent();
		LivesComponent(const LivesComponent& other) = default;
		LivesComponent(LivesComponent&& other) = default;
		LivesComponent& operator=(const LivesComponent& other) = default;
		LivesComponent& operator=(LivesComponent&& other) = default;

		virtual void OnEvent(const DeathEvent& event) override;

	private:
		void UpdateHud(const std::string& name, int nrLives);
		std::shared_ptr<TextComponent> m_pTextComponent;

		PlayerComponent* m_pPlayer;
	};
}

