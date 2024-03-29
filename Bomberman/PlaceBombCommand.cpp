#include "PlaceBombCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "SpriteRenderComponent.h"
#include "BombComponent.h"
#include "PlayerComponent.h"
#include "BombExplodedEvent.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
#include "Scene.h"
#include "BombermanManager.h"

using namespace dae;

dae::PlaceBombCommand::PlaceBombCommand(GameObject* pGameObject)
    :m_pGameObject(pGameObject), m_pPlayer{ pGameObject->GetComponent<PlayerComponent>().get() }
{
}

void PlaceBombCommand::Execute()
{
    if (!m_pGameObject) return;

    auto& manager{ BombermanManager::GetInstance() };
    const int maxNrBombs{ manager.GetMaxNrBombs(m_pPlayer->GetIndex()) };
    const int nrBombs{ m_pPlayer->GetNrBombs() };

    if (nrBombs < maxNrBombs)
    {
        glm::vec2 position{ m_pGameObject->GetWorldPosition() };

        NavigationNode* pNode{ SceneManager::GetInstance().GetCurrentScene()->GetGrid()->GetNode(position) };

        if (pNode)
        {
            //Create bomb
            const auto pBomb{ std::make_shared<GameObject>() };

            const auto pBombRenderer{ std::make_shared<SpriteRenderComponent>(false,0,3 * 16,16,16,2.f) };
            pBombRenderer->SetTexture("BombermanSprites.png");
            pBomb->AddComponent(pBombRenderer);

            const glm::vec2& nodePosition{ pNode->GetWorldPosition() };
            pBomb->SetPosition(nodePosition.x, nodePosition.y);

            constexpr float detonationTime{ 2.5f };

            const auto pBombComponent
            {
                std::make_shared<BombComponent>(detonationTime,pNode, m_pPlayer,manager.GetExplosionRange(m_pPlayer->GetIndex()))
            };

            pBomb->AddComponent(pBombComponent);

            SceneManager::GetInstance().GetCurrentScene()->Add(pBomb);
        }
    }
}


