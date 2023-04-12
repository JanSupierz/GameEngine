#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class GameObject;

	class PlayerComponent final : public Component, public Subject
	{
	public:
		PlayerComponent(const std::string& name, int priority = 0);
		virtual ~PlayerComponent() = default;
		PlayerComponent(const PlayerComponent& other) = default;
		PlayerComponent(PlayerComponent&& other) = default;
		PlayerComponent& operator=(const PlayerComponent& other) = default;
		PlayerComponent& operator=(PlayerComponent&& other) = default;

		void RemoveLife() const;
		std::string GetName() const;

	private:
		const std::string m_Name;
	};
}

