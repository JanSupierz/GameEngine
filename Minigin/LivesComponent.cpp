#include "LivesComponent.h"

#include "GameObject.h"
#include "TextComponent.h"
#include <iostream>
#include "PlayerComponent.h"
#include "PlayerDiedEvent.h"

dae::LivesComponent::LivesComponent(PlayerComponent* pPlayer, std::shared_ptr<TextComponent> pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent)
{
	UpdateHud(pPlayer->GetName(), pPlayer->GetNrLives());
}

void dae::LivesComponent::OnNotify(const PlayerDiedEvent& event)
{
	PlayerComponent* pPlayer{ event.GetKilled() };
	UpdateHud(pPlayer->GetName(), pPlayer->GetNrLives());
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
