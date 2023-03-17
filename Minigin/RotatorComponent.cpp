#include "RotatorComponent.h"
#include "SceneManager.h"
#include "GameObject.h"

void dae::RotatorComponent::FixedUpdate()
{
	const float deltaTime{ SceneManager::GetInstance().GetFixedTimeStep() };
	
	m_Angle += m_CycleSpeed * deltaTime;
	
	const float posX{ m_Origin.x + m_Radius * cos(m_Angle) };
	const float posY{ m_Origin.y + m_Radius * sin(m_Angle) };

	GetOwner()->SetPosition(posX, posY);
}

dae::RotatorComponent::RotatorComponent(const glm::vec3& origin, float radius, float nrSeconds, int priority)
	:Component(priority), m_Origin{ origin }, m_Radius{ radius }, m_CycleSpeed{ (2.f * 3.14159f) / nrSeconds }
{
}

void dae::RotatorComponent::SetLocalOrigin(const glm::vec3& origin)
{
	m_Origin = origin;
}

void dae::RotatorComponent::SetRadius(float radius)
{
	m_Radius = radius;
}

void dae::RotatorComponent::SetCycleTime(float nrSeconds)
{
	m_CycleSpeed = (2.f * 3.14159f) / nrSeconds;
}
