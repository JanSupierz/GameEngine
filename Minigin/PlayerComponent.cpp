#include "PlayerComponent.h"
#include "GameObject.h"
#include "BombComponent.h"
#include "PlayerDiedEvent.h"
#include "BombExplodedEvent.h"

using namespace dae;

std::vector<dae::PlayerComponent*> PlayerComponent::s_Players;

PlayerComponent::PlayerComponent(const std::string& name, int nrLives, int priority)
	: Component(priority), m_Name{ name }, m_Nrlives{nrLives},
	m_pPlayerDied {std::make_unique<PlayerDiedEvent>()}
{
	s_Players.push_back(this);
}

PlayerComponent::~PlayerComponent()
{
	s_Players.erase(std::remove(s_Players.begin(), s_Players.end(), this));
}

std::string PlayerComponent::GetName() const
{
	return m_Name;
}

void dae::PlayerComponent::SetScore(int score)
{
	m_Score = score;
}

int dae::PlayerComponent::GetScore() const
{
	return m_Score;
}

int dae::PlayerComponent::GetNrLives() const
{
	return m_Nrlives;
}

void dae::PlayerComponent::OnNotify(const BombExplodedEvent& event)
{
	constexpr float killDistance{ 80.f };
	BombComponent* pBomb{ event.GetExpoloded() };

	if (glm::distance(pBomb->GetOwner()->GetWorldPosition(), GetOwner()->GetWorldPosition()) <= killDistance)
	{
		--m_Nrlives;

		m_pPlayerDied->operator()(pBomb->GetPlayer(), this);
	}
}

std::vector<PlayerComponent*> PlayerComponent::GetPlayers()
{
	return s_Players;
}

PlayerDiedEvent* dae::PlayerComponent::GetPlayerDiedEvent() const
{
	return m_pPlayerDied.get();
}
