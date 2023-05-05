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

dae::BombComponent::BombComponent(float time, NavigationNode* pNode, PlayerComponent* pPlayer, int range, int priority)
	:Component(priority), m_TimeLeft{ time }, m_pPlayer{ pPlayer }, m_pNode{ pNode }, m_Range{ range }
{
	pNode->IsBlocked(true);
}

void dae::BombComponent::Update()
{
	m_TimeLeft -= SceneManager::GetInstance().GetDeltaTime();

	if (m_TimeLeft <= 0)
	{
		Explode();
	}
}

void dae::BombComponent::CreateExplosion(Scene* pScene, NavigationNode* pCurrentNode) const
{
	//Create explosions
	const auto pExplosion{ std::make_shared<GameObject>() };

	const auto pRenderComponent{ std::make_shared<SpriteRenderComponent>(0,11 * 16,16,16,2.f) };
	pRenderComponent->SetTexture("BombermanSprites.png");
	pExplosion->AddComponent(pRenderComponent);

	constexpr float duration{ 1.f };
	const auto pExplosionComponent{ std::make_shared<ExplosionComponent>(duration, m_pPlayer, pCurrentNode) };
	pExplosion->AddComponent(pExplosionComponent);

	pExplosion->SetPosition(pCurrentNode->GetWorldPosition());
	pScene->Add(pExplosion);
}

void dae::BombComponent::Explode()
{
	const auto pScene{ SceneManager::GetInstance().GetCurrentScene() };

	CreateExplosion(pScene, m_pNode);

	for (int neighbor{ static_cast<int>(Direction::begin) }; neighbor <= static_cast<int>(Direction::end); ++neighbor)
	{
		NavigationNode* pCurrentNode{ m_pNode };

		for (int index{}; index < m_Range; ++index)
		{
			auto pNode = pCurrentNode->GetNeighbor(static_cast<Direction>(neighbor));

			if (pNode)
			{
				pCurrentNode = pNode;
				CreateExplosion(pScene, pCurrentNode);
				std::cout << "Explosion!\n";
			}
		}
	}

	m_pNode->IsBlocked(false);

	GetOwner()->Destroy();
}

