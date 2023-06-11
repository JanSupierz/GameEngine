#include "PlayerComponent.h"
#include "GameObject.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "CollisionEvent.h"
#include "ColliderComponent.h"
#include "BombComponent.h"
#include <algorithm>
#include "InputManager.h"
#include "SDL.h"
#include "DetonateBombCommand.h"
#include "Controller.h"
#include "Door.h"
#include "BombermanManager.h"

using namespace dae;
int dae::PlayerComponent::s_PlayerDeathSound = -1;

PlayerComponent::PlayerComponent(int index, bool canDetonate, Subject<CollisionEvent>* pCollider, int priority)
	: Component(priority), m_Index{ index }, m_pCollider{ pCollider }, m_Killed{ false }, m_CanDetonate{ canDetonate }
{
	m_pCollider->AddObserver(this);

	if (m_CanDetonate)
	{
		CreateDetonateCommands();
	}
}

PlayerComponent::~PlayerComponent()
{
	 if (m_pCollider)
	 {
		 m_pCollider->RemoveObserver(this);
	 }
}

int dae::PlayerComponent::GetNrBombs() const
{
	return static_cast<int>(m_pBombs.size());
}

void dae::PlayerComponent::AddDetonateCommand()
{
	if (!m_CanDetonate)
	{
		m_CanDetonate = true;

		CreateDetonateCommands();
	}
}

void dae::PlayerComponent::Kill(DeathType type, PlayerComponent* pOther)
{
	if (m_Killed) return;
	m_Killed = true;
	
	EventManager::GetInstance().AddEvent(std::make_shared<DeathEvent>(type, pOther, this));
}

void dae::PlayerComponent::SetDeathSound(const int soundId)
{
	s_PlayerDeathSound = soundId;
}

void dae::PlayerComponent::DetonateOldestBomb()
{
	if (m_pBombs.size() > 0)
	{
		m_pBombs[0]->Explode();
	}
}

void dae::PlayerComponent::AddBomb(BombComponent* pBomb)
{
	m_pBombs.emplace_back(pBomb);
}

void dae::PlayerComponent::RemoveBomb(BombComponent* pBomb)
{
	m_pBombs.erase(std::remove(m_pBombs.begin(), m_pBombs.end(), pBomb), m_pBombs.end());
}

void dae::PlayerComponent::CreateDetonateCommands()
{
	auto& input{ InputManager::GetInstance() };

	input.GetKeyboard()->MapCommandToButton(SDL_SCANCODE_E, std::make_unique<DetonateBombCommand>(this), ButtonState::Down);
	input.GetController(m_Index)->MapCommandToButton(Controller::ControllerButtons::ButtonB, std::make_unique<DetonateBombCommand>(this), ButtonState::Down);
}

dae::NavigationNode* dae::PlayerComponent::GetNode() const
{
	return m_pCurrentNode;
}

void dae::PlayerComponent::SetNode(NavigationNode* pNode)
{
	m_pCurrentNode = pNode;
}

void dae::PlayerComponent::OnNotify(const CollisionEvent& event)
{
	auto& manager{ BombermanManager::GetInstance() };
	if (manager.GetNrEnemies() <= 0)
	{
		if (event.GetOtherCollider()->GetOwner()->GetComponent<Door>())
		{
			manager.NextLevel();
		}
	}
}

void dae::PlayerComponent::OnSubjectDestroy(Subject<CollisionEvent>*)
{
	m_pCollider = nullptr;
}

int dae::PlayerComponent::GetIndex() const
{
	return m_Index;
}

