#include "PlayerComponent.h"
#include "GameObject.h"
#include "BombComponent.h"
#include "PlayerDiedEvent.h"
#include "BombExplodedEvent.h"
#include "EventManager.h"
#include "NavigationGrid.h"

using namespace dae;

PlayerComponent::PlayerComponent(const glm::vec2& startPos, const std::string& name, int nrLives, int priority)
	: Component(priority), m_Name{ name }, m_Nrlives{ nrLives }, m_StartPosition{ startPos }
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

dae::NavigationNode* dae::PlayerComponent::GetNode() const
{
	return m_pCurrentNode;
}

void dae::PlayerComponent::SetNode(NavigationNode* pNode)
{
	m_pCurrentNode = pNode;
}

void dae::PlayerComponent::OnEvent(const BombExplodedEvent& event)
{
	if (event.GetNode() == NavigationGrid::GetInstance().GetNode(GetOwner()->GetWorldPosition()))
	{
		--m_Nrlives;
		EventManager::GetInstance().AddEvent(std::make_shared<PlayerDiedEvent>(event.GetPlayer(), this));

		GetOwner()->SetPosition(m_StartPosition);
	}
}
