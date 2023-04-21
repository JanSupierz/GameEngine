#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	class RotatorComponent final :public Component
	{
	public:
		virtual void Update() override;

		RotatorComponent(const glm::vec3& origin, float radius, float cycleTime, int priority = 0);
		virtual ~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = default;
		RotatorComponent(RotatorComponent&& other) = default;
		RotatorComponent& operator=(const RotatorComponent& other) = default;
		RotatorComponent& operator=(RotatorComponent&& other) = default;

		void SetLocalOrigin(const glm::vec3& origin);
		void SetRadius(float radius);
		void SetCycleTime(float nrSeconds);
	private:
		glm::vec3 m_Origin;

		float m_Radius;
		float m_CycleSpeed;
		float m_Angle{};
	};
}
