#include "BlockingComponent.h"
#include "GameObject.h"
#include "NavigationNode.h"
#include "EventManager.h"
#include "BombExplodedEvent.h"
#include "SpriteRenderComponent.h"
#include <memory>
#include "SceneManager.h"
#include "Scene.h"
#include "PowerUp.h"
#include "ColliderComponent.h"
#include "NavigationGrid.h"

dae::BlockingComponent::BlockingComponent(NavigationNode* pNode, int priority)
	:Component(priority), m_pNode{ pNode }
{
	EventManager::GetInstance().AddListener(this);
	pNode->IsBlocked(true);
}

dae::BlockingComponent::~BlockingComponent()
{
	EventManager::GetInstance().RemoveListener(this);
	m_pNode->IsBlocked(false);
}

void dae::BlockingComponent::OnEvent(const BombExplodedEvent& event)
{
	if (event.GetNode() == m_pNode)
	{
		GetOwner()->Destroy();

		//10% chance
		constexpr int discard{ 90 }, total{ 100 };
		int chance{ rand() % total };
		if (chance < discard) return;

		chance -= discard;

		constexpr int nrPowerUps{ 3 };
		PowerUpType type{ static_cast<PowerUpType>((static_cast<float>(chance) / (total - discard)) * nrPowerUps) };
		std::cout << static_cast<int>(type) << '\n';
		int spriteOffsetX{};

		switch (type)
		{
		case PowerUpType::Detonator:
			spriteOffsetX = 5;
			break;
		case PowerUpType::ExtraBomb:
			spriteOffsetX = 0;
			break;
		case PowerUpType::Flames:
			spriteOffsetX = 1;
			break;
		default:
			break;
		}

		auto pObject{ SceneManager::GetInstance().GetCurrentScene()->Add(std::make_shared<GameObject>()) };

		const float dimension{ static_cast<float>(m_pNode->GetGrid()->GetSmallerDimension()) };
		const auto pCollider{ std::make_shared<ColliderComponent>(glm::vec2{dimension, dimension}) };
		pObject->AddComponent(pCollider);

		constexpr int tileSize{ 16 };

		const auto pRenderComponent{ std::make_shared<SpriteRenderComponent>(false, spriteOffsetX * tileSize,14 * tileSize,tileSize,tileSize,2.f) };
		pRenderComponent->SetTexture("BombermanSprites.png");
		pObject->AddComponent(pRenderComponent);

		const auto pPowerUp{ std::make_shared<PowerUp>(type, pCollider->GetCollisionEvent()) };
		pObject->AddComponent(pPowerUp);

		pObject->SetPosition(m_pNode->GetWorldPosition());
	}
}

