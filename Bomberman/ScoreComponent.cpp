#include "ScoreComponent.h"
#include "TextureComponent.h"
#include <iostream>
#include "PlayerComponent.h"
#include "GainedPointEvent.h"
#include "EventManager.h"
#include "Logger.h"

dae::ScoreComponent::ScoreComponent(const std::string& name, TextureComponent* pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent), m_Name{ name }
{
	EventManager::GetInstance().AddListener(this);
}

dae::ScoreComponent::~ScoreComponent()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::ScoreComponent::OnEvent(const GainedPointEvent& event)
{
	if (event.GetName() == m_Name)
	{
		std::string text{ m_Name + ": " + std::to_string(event.GetScore()) + " score" };

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
