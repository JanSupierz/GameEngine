#pragma once
#include "Component.h"
#include "Direction.h"
#include <memory>

namespace dae
{
	class NavigationNode;
	class UpdatePosition2DCommand;

	class AIMoveComponent final : public Component
	{
	public:
		virtual void Update() override;

		AIMoveComponent(float speed, int priority = 0);
		virtual ~AIMoveComponent() = default;
		AIMoveComponent(const AIMoveComponent& other) = default;
		AIMoveComponent(AIMoveComponent&& other) = default;
		AIMoveComponent& operator=(const AIMoveComponent& other) = default;
		AIMoveComponent& operator=(AIMoveComponent&& other) = default;
	private:
		NavigationNode* m_pCurrentNode;
		Direction m_MoveDirection{};
		std::unique_ptr<UpdatePosition2DCommand> m_pMoveCommand{};
		float m_Speed{};
	};
}

