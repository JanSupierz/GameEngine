#include "Component.h"
#include <iostream>
#include "GameObject.h"

bool dae::Component::operator<(const Component& other) const
{
	return m_Priority < other.m_Priority;
}

void dae::Component::SetPriority(int priority)
{
	m_Priority = priority;
}

int dae::Component::GetPriority() const
{
	return m_Priority;
}

void dae::Component::AddToGameObject(GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
}

void dae::Component::RemoveFromGameObject(GameObject* pGameObject)
{
	if (m_pGameObject == pGameObject)
	{
		m_pGameObject = nullptr;
	}
}

void dae::Component::Update()
{
}

void dae::Component::FixedUpdate()
{
}

void dae::Component::Render()
{
}

dae::Component::Component(int priority)
	:m_Priority{ priority }
{
}
