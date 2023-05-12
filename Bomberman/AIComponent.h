#pragma once
#include "Component.h"
#include "Direction.h"
#include <memory>

namespace dae
{
	class NavigationNode;
	class AIWalkCommand;

	class AIComponent final : public Component
	{
	public:
		virtual void Update() override;

		AIComponent(std::unique_ptr<AIWalkCommand>&& pMoveCommand, int priority = 0);
		virtual ~AIComponent() = default;
		AIComponent(const AIComponent& other) = default;
		AIComponent(AIComponent&& other) = default;
		AIComponent& operator=(const AIComponent& other) = default;
		AIComponent& operator=(AIComponent&& other) = default;
	private:
		std::unique_ptr<AIWalkCommand> m_pMoveCommand{};
	};
}

