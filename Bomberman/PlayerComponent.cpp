#include "PlayerComponent.h"
#include "GameObject.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "CollisionEvent.h"
#include "ColliderComponent.h"
#include "ExplosionComponent.h"
#include "Audio.h"

using namespace dae;
int dae::PlayerComponent::s_PlayerDeathSound = -1;

PlayerComponent::PlayerComponent(const glm::vec2& startPos, const std::string& name, int nrLives, Subject<CollisionEvent>* pCollider, int priority)
	: Component(priority), m_Name{ name }, m_Nrlives{ nrLives }, m_StartPosition{ startPos }, m_pCollider{ pCollider }
{
	m_pCollider->AddObserver(this);
}

PlayerComponent::~PlayerComponent()
{
	 if (m_pCollider)
	 {
		 m_pCollider->RemoveObserver(this);
	 }
}

std::string PlayerComponent::GetName() const
{
	return m_Name;
}

void dae::PlayerComponent::SetScore(int score)
{
	m_Score = score;
}

void dae::PlayerComponent::Kill(DeathType type, PlayerComponent* pOther)
{
	--m_Nrlives;
	GetOwner()->SetPosition(m_StartPosition);

	EventManager::GetInstance().AddEvent(std::make_shared<DeathEvent>(type, pOther, this));

	Audio::Get().Play(s_PlayerDeathSound, 0.5f);
}

void dae::PlayerComponent::SetDeathSound(const int soundId)
{
	s_PlayerDeathSound = soundId;
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

void dae::PlayerComponent::OnNotify(const CollisionEvent&)
{
	//Listen for collision with doors
}

void dae::PlayerComponent::OnSubjectDestroy(Subject<CollisionEvent>*)
{
	m_pCollider = nullptr;
}
