#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	class SineMovementComponent final :public Component
	{
	public:
		virtual void FixedUpdate() override;

		SineMovementComponent(const glm::vec3& origin, float length, float cycleTime, int priority = 0);
		virtual ~SineMovementComponent() = default;
		SineMovementComponent(const SineMovementComponent& other) = default;
		SineMovementComponent(SineMovementComponent&& other) = default;
		SineMovementComponent& operator=(const SineMovementComponent& other) = default;
		SineMovementComponent& operator=(SineMovementComponent&& other) = default;

	private:
		glm::vec3 m_Origin;

		float m_Length;
		float m_CycleSpeed;
		float m_Angle{};
	};
}
