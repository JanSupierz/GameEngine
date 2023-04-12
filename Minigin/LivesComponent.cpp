#include "LivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <iostream>
#include "PlayerComponent.h"

dae::LivesComponent::LivesComponent(int nrLives, std::shared_ptr<TextComponent> pTextComponent, int priority)
	:Component(priority), m_NrLives(nrLives), m_pTextComponent(pTextComponent)
{
}

void dae::LivesComponent::OnNotify(const void* pEntity, const std::string& event)
{
	if (event == "PlayerDied")
	{
		--m_NrLives;

		const auto pPlayer{ static_cast<const PlayerComponent*>(pEntity) };
		std::string text{ pPlayer->GetName() + ": " + std::to_string(m_NrLives) + " lives" };

		if (m_pTextComponent)
		{
			m_pTextComponent->SetTextToTexture(text);
		}
		else
		{
			std::cout << text << '\n';
		}
	}
}