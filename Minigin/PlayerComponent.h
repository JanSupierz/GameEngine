#pragma once
#include "Component.h"
#include "Subject.h"
#include "Observer.h"
#include <vector>

namespace dae
{
	class GameObject;

	class PlayerComponent final : public Component, public Subject, public Observer, public std::enable_shared_from_this<PlayerComponent>
	{
	public:
		PlayerComponent(const std::string& name, int priority = 0);
		virtual ~PlayerComponent();
		PlayerComponent(const PlayerComponent& other) = default;
		PlayerComponent(PlayerComponent&& other) = default;
		PlayerComponent& operator=(const PlayerComponent& other) = default;
		PlayerComponent& operator=(PlayerComponent&& other) = default;

		void Start() const;
		std::string GetName() const;

		static std::vector<PlayerComponent*> GetPlayers();

		virtual void OnNotify(const void* pData, const std::string& event);
	private:
		const std::string m_Name;
		static std::vector<dae::PlayerComponent*> s_Players;
	};
}