#include "Transform.h"

void dae::Transform::UpdateWorldPosition(const glm::vec3& position)
{
	m_WorldPosition = position;
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
}