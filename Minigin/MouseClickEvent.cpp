#include "MouseClickEvent.h"
#include "Mouse.h"

dae::MouseClickEvent::MouseClickEvent(const glm::vec2& mousePos, MouseButton button)
	:m_MousePos{ mousePos }, m_Button{ button }
{
}

glm::vec2 dae::MouseClickEvent::GetMousePos() const
{
	return m_MousePos;
}

dae::MouseButton dae::MouseClickEvent::GetMouseButton() const
{
	return m_Button;
}
