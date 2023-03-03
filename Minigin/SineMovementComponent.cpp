#include "SineMovementComponent.h"
#include "SceneManager.h"
#include "GameObject.h"

void dae::SineMovementComponent::FixedUpdate()
{
	const float deltaTime{ SceneManager::GetInstance().GetFixedTimeStep() };
	
	m_Angle += m_CycleSpeed * deltaTime;

	const float posX{ m_Origin.x + m_Length * cos(m_Angle) };
	const float posY{ m_Origin.y + m_Length * sin(m_Angle) };

	GetOwner()->SetPosition(posX, posY);
}

dae::SineMovementComponent::SineMovementComponent(const glm::vec3& origin, float length, float cycleTime, int priority)
	:Component(priority), m_Origin{ origin }, m_Length{ length }, m_CycleSpeed{ 2.f * 3.14159f / cycleTime }
{
}
