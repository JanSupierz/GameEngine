#pragma once
#include "Component.h"
#include "EventListener.h"
#include <string>

namespace dae
{
	class GameObject;
	class TextureComponent;
	class PlayerComponent;
	class HUDEvent;

	class LivesComponent final : public Component, public EventListener<HUDEvent>
	{
	public:
		LivesComponent(int index, TextureComponent* pTextComponent, int priority = 0);
		virtual ~LivesComponent();
		LivesComponent(const LivesComponent& other) = default;
		LivesComponent(LivesComponent&& other) = default;
		LivesComponent& operator=(const LivesComponent& other) = default;
		LivesComponent& operator=(LivesComponent&& other) = default;

		virtual void OnEvent(const HUDEvent& event) override;

	private:
		TextureComponent* m_pTextComponent;
		const int m_Index;
	};
}

