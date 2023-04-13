#pragma once
#include "Component.h"
#include "Observer.h"
#include "Subject.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class PlayerComponent;

	class ScoreComponent final : public Component, public Observer, public Subject
	{
	public:
		ScoreComponent(std::shared_ptr<PlayerComponent> pPlayer, std::shared_ptr<TextComponent> pTextComponent = nullptr, int priority = 0);
		virtual ~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = default;
		ScoreComponent(ScoreComponent&& other) = default;
		ScoreComponent& operator=(const ScoreComponent& other) = default;
		ScoreComponent& operator=(ScoreComponent&& other) = default;

		virtual void OnNotify(const void* pData, const std::string& event) override;

	private:
		void UpdateHUD(const std::string& name);

		std::shared_ptr<TextComponent> m_pTextComponent;
		std::shared_ptr<PlayerComponent> m_pPlayer;
		int m_Score{};
	};
}

