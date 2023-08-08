#include "BombComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerComponent.h"
#include "NavigationGrid.h"
#include "SpriteRenderComponent.h"
#include "ExplosionComponent.h"
#include "NavigationNode.h"
#include <iostream>
#include "EventManager.h"
#include "BombExplodedEvent.h"
#include "Audio.h"
#include "ColliderComponent.h"

 int dae::BombComponent::s_ExplosionSoundId = -1;

dae::BombComponent::BombComponent(float time, NavigationNode* pNode, PlayerComponent* pPlayer, int range, int priority)
	:Component(priority), m_TimeLeft{ time }, m_pPlayer{ pPlayer }, m_pNode{ pNode }, m_Range{ range }
{
	pNode->IsBlocked(true);
	EventManager::GetInstance().AddListener(this);

	m_pPlayer->AddBomb(this);
}

dae::BombComponent::~BombComponent()
{
	m_pNode->IsBlocked(false);
	EventManager::GetInstance().RemoveListener(this);
}

void dae::BombComponent::Update()
{
	m_TimeLeft -= SceneManager::GetInstance().GetDeltaTime();

	if (m_TimeLeft <= 0)
	{
		Explode();
	}
}

void dae::BombComponent::CreateExplosion(Scene* pScene, NavigationNode* pNode, Direction direction, bool shouldStop) const
{
	//Create explosions
	const auto pExplosion{ std::make_shared<GameObject>() };

	const float dimension{ static_cast<float>(pNode->GetGrid()->GetSmallerDimension())};
	const auto pCollider{ std::make_shared<ColliderComponent>(glm::vec2{dimension, dimension}) };
	pExplosion->AddComponent(pCollider);

	constexpr int tileSize{ 16 };
	int posX{ 2 * tileSize }, posY{ 11 * tileSize };

	const int offset{ shouldStop ? tileSize * 2 : tileSize };

	switch (direction)
	{
	case Direction::up:
		posY += offset;
		break;
	case Direction::down:
		posY -= offset;
		break;
	case Direction::left:
		posX -= offset;
		break;
	case Direction::right:
		posX += offset;
		break;
	}

	const auto pRenderComponent{ std::make_shared<SpriteRenderComponent>(false, posX,posY,tileSize,tileSize,2.f) };
	pRenderComponent->SetTexture("BombermanSprites.png");
	pExplosion->AddComponent(pRenderComponent);

	constexpr float duration{ 0.5f };
	const auto pExplosionComponent{ std::make_shared<ExplosionComponent>(duration, m_pPlayer, pNode, pCollider->GetCollisionEvent()) };
	pExplosion->AddComponent(pExplosionComponent);

	pExplosion->SetPosition(pNode->GetWorldPosition());
	pScene->Add(pExplosion);

	EventManager::GetInstance().AddEvent(std::make_unique<BombExplodedEvent>(pNode, m_pPlayer));
}

void dae::BombComponent::OnEvent(const BombExplodedEvent& event)
{
	if (event.GetNode() == m_pNode)
	{
		Explode();
	}
}

void dae::BombComponent::SetExplosionSound(const int soundId)
{
	s_ExplosionSoundId = soundId;
}

void dae::BombComponent::Explode()
{
	if (m_IsExploded) return;
	m_IsExploded = true;

	const auto pScene{ SceneManager::GetInstance().GetCurrentScene() };

	CreateExplosion(pScene, m_pNode, Direction::NONE, false);

	for (int neighbor{ static_cast<int>(Direction::begin) }; neighbor <= static_cast<int>(Direction::end); ++neighbor)
	{
		NavigationNode* pCurrentNode{ m_pNode };

		for (int index{}; index < m_Range; ++index)
		{
			auto pNode = pCurrentNode->GetNeighbor(static_cast<Direction>(neighbor));

			if (pNode)
			{
				pCurrentNode = pNode;

				const bool shouldStop{ pNode->IsBlocked() };

				CreateExplosion(pScene, pCurrentNode, static_cast<Direction>(neighbor), shouldStop || m_Range == index + 1);

				if (shouldStop)
				{
					break;
				}
			}
		}
	}

	Audio::Get().Play(s_ExplosionSoundId, 0.5f);

	m_pPlayer->RemoveBomb(this);

	GetOwner()->Destroy();
}

