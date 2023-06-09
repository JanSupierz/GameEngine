#include "ButtonComponent.h"
#include "EventManager.h"
#include "GameObject.h"
#include "MouseClickEvent.h"
#include "Mouse.h"
#include "Logger.h"

dae::ButtonComponent::ButtonComponent(const std::function<void()>& callBack, int width, int height)
	:Component(0), m_CallBack{ callBack }, m_Width{ width }, m_Height{ height }
{
	EventManager::GetInstance().AddListener(this);
}

dae::ButtonComponent::~ButtonComponent()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::ButtonComponent::OnEvent(const MouseClickEvent& event)
{
	if (event.GetMouseButton() != MouseButton::left) return;

	const auto difference{ event.GetMousePos() - GetOwner()->GetWorldPosition() };

	if (difference.x > 0.f && difference.x < m_Width 
		&& difference.y > 0.f && difference.y < m_Height)
	{
		m_CallBack();
	}
}