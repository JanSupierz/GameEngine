#include "LivesComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include <iostream>
#include "PlayerComponent.h"
#include "DeathEvent.h"
#include "EventManager.h"

dae::LivesComponent::LivesComponent(PlayerComponent* pPlayer, std::shared_ptr<TextComponent> pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent), m_pPlayer(pPlayer)
{
	EventManager::GetInstance().AddListener(this);
	UpdateHud(pPlayer->GetName(), pPlayer->GetNrLives());
}

dae::LivesComponent::~LivesComponent()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::LivesComponent::OnEvent(const DeathEvent& event)
{
	if (m_pPlayer == event.GetKilled())
	{
		UpdateHud(m_pPlayer->GetName(), m_pPlayer->GetNrLives());
	}
}

void dae::LivesComponent::UpdateHud(const std::string& name, int nrLives)
{
	std::string text{ name + ": " + std::to_string(nrLives) + " lives"};

	if (m_pTextComponent)
	{
		m_pTextComponent->SetTextToTexture(text);
	}
	else
	{
		std::cout << text << '\n';
	}
}
