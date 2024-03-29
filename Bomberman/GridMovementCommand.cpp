#include "GridMovementCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
#include <iostream>
#include "PlayerComponent.h"
#include "Scene.h"

using namespace dae;

GridMovementCommand::GridMovementCommand(GameObject* pGameObject, const glm::vec2& direction, PlayerComponent* pPlayer, bool useValue2D)
    : m_pGameObject(pGameObject),
    m_Direction(glm::normalize(direction)),
    m_Speed{ glm::length(direction) },
    m_pSceneManager(&SceneManager::GetInstance()),
    m_pGrid{ m_pSceneManager->GetCurrentScene()->GetGrid() }, 
    m_pPlayer{ pPlayer },
    m_UseValue2D{ useValue2D }
{};

void GridMovementCommand::Execute()
{
    if (!m_pGameObject) return;

    if (m_UseValue2D)
    {
        if (abs(m_Value.x) > abs(m_Value.y))
        {
            m_Direction.x = (m_Value.x > 0.f ? 1.f: -1.f);
            m_Direction.y = 0.f;
        }
        else
        {
            m_Direction.x = 0.f;
            m_Direction.y = (m_Value.y > 0.f ? 1.f: -1.f);
        }
    }

    NavigationNode* pCurrentNode{ m_pPlayer->GetNode() };
    const float speedDelta{ m_Speed * m_pSceneManager->GetDeltaTime() };

    //Get new position
    const glm::vec2 oldPosition{ m_pGameObject->GetLocalPosition() };
    glm::vec2 newPosition{ oldPosition + m_Direction * speedDelta };

    //Check if we can walk further
    NavigationNode* pNode{ m_pGrid->GetNode(newPosition + m_Direction * (m_pGrid->GetSmallerDimension() * 0.5f)) };

    //Move to the center of the cell
    if (pNode)
    {
        constexpr float epsilon{ 0.1f };
        constexpr float minDistance{ 1.f };

        //Distance from new positon to the center
        const glm::vec2 distance{ pNode->GetWorldPosition() - newPosition };

        int axis{ -1 };

        //Move in the y axis
        if (abs(m_Direction.x) > epsilon && abs(distance.y) > minDistance)
        {
            axis = 1;
        }
        //Move in the x axis
        else if(abs(distance.x) > minDistance)
        {
            axis = 0;
        }

        //Don't walk, distance is too small
        if(axis != -1)
        {
            //Check the direction sign
            const int sign{ (pNode->GetWorldPosition()[axis] > newPosition[axis] ? 1 : -1)};

            //Move to the center
            const float offset{ sign * std::min(abs(distance[axis]), speedDelta)};
            newPosition[axis] = oldPosition[axis] + offset;

            //Prevent speeding up on the corners
            newPosition = oldPosition + glm::normalize(newPosition - oldPosition)* speedDelta;
        }

        //Move to the destination
        if ((pNode == pCurrentNode) || (pNode && !pNode->IsBlocked()))
        {
            m_pGameObject->SetPosition(newPosition.x, newPosition.y);
            m_pPlayer->SetNode(pNode);
        }
        else if (m_pGrid->GetNode(oldPosition) == pNode)
        {
            m_pGameObject->SetPosition(newPosition.x, newPosition.y);
        }
    }
}