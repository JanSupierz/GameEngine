#include "PlayerComponent.h"
#include "GameObject.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "CollisionEvent.h"
#include "ColliderComponent.h"
#include "ExplosionComponent.h"
#include "Audio.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraComponent.h"

using namespace dae;
int dae::PlayerComponent::s_PlayerDeathSound = -1;

PlayerComponent::PlayerComponent(const std::string& name, Subject<CollisionEvent>* pCollider, int priority)
	: Component(priority), m_Name{ name }, m_pCollider{ pCollider }, m_Killed{ false }
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

void dae::PlayerComponent::Kill(DeathType type, PlayerComponent* pOther)
{
	if (m_Killed) return;

	m_Killed = true;
	
	EventManager::GetInstance().AddEvent(std::make_shared<DeathEvent>(type, pOther, this));

	Audio::Get().Play(s_PlayerDeathSound, 0.5f);

	SceneManager::GetInstance().GetCurrentScene()->Load(true);
}

void dae::PlayerComponent::SetDeathSound(const int soundId)
{
	s_PlayerDeathSound = soundId;
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
