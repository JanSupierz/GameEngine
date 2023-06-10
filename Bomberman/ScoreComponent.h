#pragma once
#include "Component.h"
#include "EventListener.h"
#include <string>

namespace dae
{
	class TextureComponent;
	class HUDEvent;

	class ScoreComponent final : public Component, public EventListener<HUDEvent>
	{
	public:
		ScoreComponent(int index, TextureComponent* pTextComponent = nullptr, int priority = 0);
		virtual ~ScoreComponent();
		ScoreComponent(const ScoreComponent& other) = default;
		ScoreComponent(ScoreComponent&& other) = default;
		ScoreComponent& operator=(const ScoreComponent& other) = default;
		ScoreComponent& operator=(ScoreComponent&& other) = default;

		virtual void OnEvent(const HUDEvent& event) override;

	private:
		TextureComponent* m_pTextComponent;
		const int m_Index;
	};
}

