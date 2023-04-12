#include "PlayerComponent.h"
#include "GameObject.h"

dae::PlayerComponent::PlayerComponent(const std::string& name, int priority)
	:Component(priority), m_Name{ name }
{
}

void dae::PlayerComponent::RemoveLife() const
{
	Notify(this, "PlayerDied");
}

std::string dae::PlayerComponent::GetName() const
{
	return m_Name;
}
