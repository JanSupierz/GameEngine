#include "LivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <iostream>
#include "PlayerComponent.h"

dae::LivesComponent::LivesComponent(int nrLives, std::shared_ptr<TextComponent> pTextComponent, int priority)
	:Component(priority), m_NrLives(nrLives), m_pTextComponent(pTextComponent)
{
}

void dae::LivesComponent::OnNotify(const void* pData, const std::string& event)
{
	if (event == "PlayerDied")
	{
		--m_NrLives;

		const auto pPlayerPair{ static_cast<const std::pair<PlayerComponent*, PlayerComponent*>*>(pData) };
		const auto pPlayer{ pPlayerPair->second };

		TextToTexture(pPlayer->GetName());
		return;
	}

	if (event == "Start")
	{
		const auto pPlayer{ static_cast<const PlayerComponent*>(pData) };

		TextToTexture(pPlayer->GetName());
		return;
	}

	//Test add/remove observer
	if (event == "AddedObserver") //"RemovedObserver"
	{
		TextToTexture("Subject");
		return;
	}
}

void dae::LivesComponent::TextToTexture(const std::string& name)
{
	std::string text{ name + ": " + std::to_string(m_NrLives) + " lives" };

	if (m_pTextComponent)
	{
		m_pTextComponent->SetTextToTexture(text);
	}
	else
	{
		std::cout << text << '\n';
	}
}
