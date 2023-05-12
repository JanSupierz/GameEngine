#pragma once
#include "Component.h"
#include "EventListener.h"
#include <memory>
#include <vector>
#include <string>

namespace dae
{
	class PlayerDiedEvent;
	class BombExplodedEvent;
	class NavigationNode;

	class PlayerComponent final : public Component, public EventListener<BombExplodedEvent>
	{
	public:
		PlayerComponent(const std::string& name, int nrLives, int priority = 0);
		virtual ~PlayerComponent();
		PlayerComponent(const PlayerComponent& other) = default;
		PlayerComponent(PlayerComponent&& other) = default;
		PlayerComponent& operator=(const PlayerComponent& other) = default;
		PlayerComponent& operator=(PlayerComponent&& other) = default;

		void OnEvent(const BombExplodedEvent& event) override;

		std::string GetName() const;
		int GetScore() const;
		int GetNrLives() const;

		NavigationNode* GetNode() const;
		void SetNode(NavigationNode* pNode);

		void SetScore(int score);

	private:
		const std::string m_Name;
		int m_Score{};
		int m_Nrlives{};

		NavigationNode* m_pCurrentNode{ nullptr };
	};
}