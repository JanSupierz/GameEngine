#include "Transform.h"

void dae::Transform::UpdateWorldPosition(const glm::vec2& position)
{
	m_WorldPosition = position;
}

void dae::Transform::SetPosition(const float x, const float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
}