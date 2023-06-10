#include "ScoreComponent.h"
#include "TextureComponent.h"
#include <iostream>
#include "HUDEvent.h"
#include "EventManager.h"
#include "Logger.h"

dae::ScoreComponent::ScoreComponent(int index, TextureComponent* pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent), m_Index{ index }
{
	EventManager::GetInstance().AddListener(this);
}

dae::ScoreComponent::~ScoreComponent()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::ScoreComponent::OnEvent(const HUDEvent& event)
{
	if (event.GetType() == HUDEventType::Score && event.GetIndex() == m_Index)
	{
		std::string text{ "Score: " + std::to_string(event.GetValue())};

		if (m_pTextComponent)
		{
			m_pTextComponent->SetTextToTexture(text);
		}
		else
		{
			Logger::Get().Log(text);
		}
	}
}
