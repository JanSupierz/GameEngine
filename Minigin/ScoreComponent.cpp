#include "ScoreComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <iostream>
#include "PlayerComponent.h"

dae::ScoreComponent::ScoreComponent(std::shared_ptr<PlayerComponent> pPlayer, std::shared_ptr<TextComponent> pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent), m_Score{}, m_pPlayer{ pPlayer }
{
	UpdateHUD(m_pPlayer->GetName());
}

void dae::ScoreComponent::OnNotify(const void* pData, const std::string& event)
{
	if (event == "GainedPoint")
	{
		const auto pPlayer{ static_cast<const PlayerComponent*>(pData) };

		if (pPlayer != m_pPlayer.get()) return;

		++m_Score;
		UpdateHUD(pPlayer->GetName());

		Notify(&m_Score, "ScoreChanged");

		return;
	}
}

void dae::ScoreComponent::UpdateHUD(const std::string& name)
{
	std::string text{ name + ": " + std::to_string(m_Score) + " score" };

	if (m_pTextComponent)
	{
		m_pTextComponent->SetTextToTexture(text);
	}
	else
	{
		std::cout << text << '\n';
	}
}
