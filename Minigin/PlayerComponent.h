#pragma once
#include "Component.h"
#include "Observer.h"
#include <memory>
#include <vector>
#include <string>

namespace dae
{
	class PlayerDiedEvent;
	class BombExplodedEvent;
	class GameObject;

	class PlayerComponent final : public Component, public Observer<BombExplodedEvent>, public std::enable_shared_from_this<PlayerComponent>
	{
	public:
		PlayerComponent(const std::string& name, int nrLives, int priority = 0);
		virtual ~PlayerComponent();
		PlayerComponent(const PlayerComponent& other) = default;
		PlayerComponent(PlayerComponent&& other) = default;
		PlayerComponent& operator=(const PlayerComponent& other) = default;
		PlayerComponent& operator=(PlayerComponent&& other) = default;

		PlayerDiedEvent* GetPlayerDiedEvent() const;
		virtual void OnNotify(const BombExplodedEvent& event) override;

		static std::vector<PlayerComponent*> GetPlayers();

		std::string GetName() const;
		int GetScore() const;
		int GetNrLives() const;

		void SetScore(int score);

	private:
		const std::string m_Name;
		std::unique_ptr<PlayerDiedEvent> m_pPlayerDied;
		int m_Score{};
		int m_Nrlives{};

		static std::vector<PlayerComponent*> s_Players;
	};
}