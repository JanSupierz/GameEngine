#include "PlaceBombCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "BombComponent.h"
#include "PlayerComponent.h"
#include "BombExplodedEvent.h"

using namespace dae;

dae::PlaceBombCommand::PlaceBombCommand(GameObject* pGameObject)
    :m_pGameObject(pGameObject)
{
}

void PlaceBombCommand::Execute()
{
    if (!m_pGameObject) return;

    glm::vec3 position = m_pGameObject->GetWorldPosition();

    const auto pScene{ SceneManager::GetInstance()->GetCurrentScene() };

    //Create bomb
	const auto pBomb{ std::make_shared<GameObject>() };
	pBomb->SetPosition(position.x, position.y);

	const auto pBombRenderer{ std::make_shared<RenderComponent>() };
    pBombRenderer->SetTexture("Bomb.png");
	pBomb->AddComponent(pBombRenderer);

    const auto pBombComponent{ std::make_shared<BombComponent>(3.f, m_pGameObject->GetComponent<PlayerComponent>()) };
    pBomb->AddComponent(pBombComponent);

    //Make all players observe the bomb
    for (const auto pPlayer : PlayerComponent::GetPlayers())
    {
        pBombComponent->GetExplodeEvent()->AddObserver(pPlayer->shared_from_this());
    }

    pScene->Add(pBomb);
}


