#include "LivesComponent.h"
#include "TextureComponent.h"
#include <iostream>
#include "HUDEvent.h"
#include "EventManager.h"

dae::LivesComponent::LivesComponent(int index, TextureComponent* pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent), m_Index(index)
{
	EventManager::GetInstance().AddListener(this);
}

dae::LivesComponent::~LivesComponent()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::LivesComponent::OnEvent(const HUDEvent& event)
{
	if (event.GetType() == HUDEventType::Life && m_Index == event.GetIndex())
	{
		std::string text{ "Lives: " + std::to_string(event.GetValue())};
	
		if (m_pTextComponent)
		{
			m_pTextComponent->SetTextToTexture(text);
		}
	}
}