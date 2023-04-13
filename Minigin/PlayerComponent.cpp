#include "PlayerComponent.h"
#include "GameObject.h"
#include "BombComponent.h"

using namespace dae;

std::vector<dae::PlayerComponent*> PlayerComponent::s_Players;

PlayerComponent::PlayerComponent(const std::string& name, int priority)
	: Component(priority), m_Name{ name }
{
	s_Players.push_back(this);
}

PlayerComponent::~PlayerComponent()
{
	s_Players.erase(std::remove(s_Players.begin(), s_Players.end(), this));
}

void PlayerComponent::Start() const
{
	Notify(this, "Start");
}

std::string PlayerComponent::GetName() const
{
	return m_Name;
}

std::vector<PlayerComponent*> PlayerComponent::GetPlayers()
{
	return s_Players;
}

void PlayerComponent::OnNotify(const void* pData, const std::string& event)
{
	if (event == "BombExploded")
	{
		const auto pBomb{ static_cast<const BombComponent*> (pData) };
		constexpr float killDistance{ 80.f };

		if (glm::distance(pBomb->GetOwner()->GetWorldPosition(), GetOwner()->GetWorldPosition()) <= killDistance)
		{
			const auto pPlayer{ pBomb->GetPlayer() };
			const auto playerPair{ std::pair < PlayerComponent*, PlayerComponent*>{pPlayer,this} };
			Notify(&playerPair, "PlayerDied");
		}

		return;
	}
}