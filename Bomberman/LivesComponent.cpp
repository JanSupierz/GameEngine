#include "LivesComponent.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include <iostream>
#include "PlayerComponent.h"
#include "DeathEvent.h"
#include "EventManager.h"

dae::LivesComponent::LivesComponent(PlayerComponent* pPlayer, std::shared_ptr<TextureComponent> pTextComponent, int priority)
	:Component(priority), m_pTextComponent(pTextComponent), m_pPlayer(pPlayer)
{
	EventManager::GetInstance().AddListener(this);
}

dae::LivesComponent::~LivesComponent()
{
	EventManager::GetInstance().RemoveListener(this);
}

void dae::LivesComponent::OnEvent(const DeathEvent& )
{
	//if (m_Name == event.GetKilled()->GetName())
	//{
	//	std::string text{ m_Name + ": " + std::to_string(nrLives) + " lives" };
	//
	//	if (m_pTextComponent)
	//	{
	//		m_pTextComponent->SetTextToTexture(text);
	//	}
	//	else
	//	{
	//		std::cout << text << '\n';
	//	}
	//}
}