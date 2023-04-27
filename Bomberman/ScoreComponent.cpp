#include "ScoreComponent.h"
#include "TextComponent.h"
#include <iostream>
#include "PlayerComponent.h"
#include "GainedPointEvent.h"
#include "EventManager.h"

dae::ScoreComponent::ScoreComponent(std::shared_ptr<PlayerComponent> pPlayer, std::shared_ptr<TextComponent> pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent), m_pPlayer{ pPlayer }
{
	EventManager::GetInstance().AddListener(this);
	UpdateHUD();
}

dae::ScoreComponent::~ScoreComponent()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::ScoreComponent::OnEvent(const GainedPointEvent& event)
{
	const auto pPlayer{ event.GetPlayer() };
	
	//Is my player
	if (pPlayer == m_pPlayer.get())
	{
		UpdateHUD();
	}
}

void dae::ScoreComponent::UpdateHUD()
{
	std::string text{ m_pPlayer->GetName() + ": " + std::to_string(m_pPlayer->GetScore()) + " score"};

	if (m_pTextComponent)
	{
		m_pTextComponent->SetTextToTexture(text);
	}
	else
	{
		std::cout << text << '\n';
	}
}
