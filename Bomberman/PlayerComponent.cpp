#include "PlayerComponent.h"
#include "GameObject.h"
#include "BombComponent.h"
#include "PlayerDiedEvent.h"
#include "BombExplodedEvent.h"
#include "EventManager.h"

using namespace dae;

PlayerComponent::PlayerComponent(const std::string& name, int nrLives, int priority)
	: Component(priority), m_Name{ name }, m_Nrlives{ nrLives }
{
	EventManager::GetInstance().AddListener(this);
}

PlayerComponent::~PlayerComponent()
{
	 EventManager::GetInstance().RemoveListener(this);
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

void dae::PlayerComponent::OnEvent(const BombExplodedEvent& event)
{
	constexpr float killDistance{ 80.f };

	if (glm::distance(event.GetPosition(), GetOwner()->GetWorldPosition()) <= killDistance)
	{
		--m_Nrlives;
		EventManager::GetInstance().AddEvent(std::make_shared<PlayerDiedEvent>(event.GetPlayer(), this));
	}
}
