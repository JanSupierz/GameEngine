#pragma once
#include "Component.h"
#include "EventListener.h"
#include <string>

namespace dae
{
	class TextureComponent;
	class GainedPointEvent;

	class ScoreComponent final : public Component, public EventListener<GainedPointEvent>
	{
	public:
		ScoreComponent(const std::string& name, TextureComponent* pTextComponent = nullptr, int priority = 0);
		virtual ~ScoreComponent();
		ScoreComponent(const ScoreComponent& other) = default;
		ScoreComponent(ScoreComponent&& other) = default;
		ScoreComponent& operator=(const ScoreComponent& other) = default;
		ScoreComponent& operator=(ScoreComponent&& other) = default;

		virtual void OnEvent(const GainedPointEvent& event) override;

	private:
		TextureComponent* m_pTextComponent;
		std::string m_Name;
	};
}

