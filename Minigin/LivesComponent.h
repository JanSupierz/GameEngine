#pragma once
#include "Component.h"
#include "Observer.h"
#include <string>
#include <memory>

namespace dae
{
	class GameObject;
	class TextComponent;
	class PlayerComponent;
	class PlayerDiedEvent;

	class LivesComponent final : public Component, public Observer<PlayerDiedEvent>
	{
	public:
		LivesComponent(PlayerComponent* pPlayer, std::shared_ptr<TextComponent> pTextComponent = nullptr, int priority = 0);
		virtual ~LivesComponent() = default;
		LivesComponent(const LivesComponent& other) = default;
		LivesComponent(LivesComponent&& other) = default;
		LivesComponent& operator=(const LivesComponent& other) = default;
		LivesComponent& operator=(LivesComponent&& other) = default;

		virtual void OnNotify(const PlayerDiedEvent& event) override;

	private:
		void UpdateHud(const std::string& name, int nrLives);
		std::shared_ptr<TextComponent> m_pTextComponent;
	};
}

