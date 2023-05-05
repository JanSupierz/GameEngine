#include "GridMovementCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"

using namespace dae;

GridMovementCommand::GridMovementCommand(GameObject* pGameObject, const glm::vec2& direction)
    : m_pGameObject(pGameObject),
    m_Direction(glm::normalize(direction)),
    m_Speed{ glm::length(direction)},
    m_pSceneManager(&SceneManager::GetInstance()),
    m_pGrid{ &NavigationGrid::GetInstance() }
{};

void GridMovementCommand::Execute()
{
    if (!m_pGameObject) return;

    const float speedDelta{ m_Speed * m_pSceneManager->GetDeltaTime() };

    const glm::vec2 oldPosition{ m_pGameObject->GetLocalPosition() };
    glm::vec2 newPosition{ oldPosition + m_Direction * speedDelta };

    NavigationNode* pNode{ m_pGrid->GetNode(newPosition + m_Direction * (m_pGrid->GetSmallerDimension() * 0.5f)) };

    //Move to the center of the cell
    if (pNode)
    {
        constexpr float epsilon{ 0.1f };
        constexpr float minDistance{ 1.f };

        const glm::vec2 distance{ pNode->GetWorldPosition() - newPosition };

        int index{ -1 };

        if (abs(m_Direction.x) > epsilon && abs(distance.y) > minDistance)
        {
            index = 1;
        }
        else if(abs(distance.x) > minDistance)
        {
            index = 0;
        }

        if(index != -1)
        {
            const float direction{ (pNode->GetWorldPosition()[index] > newPosition[index] ? 1.f : -1.f)};

            const float offset{ direction * std::min(abs(distance[index]), speedDelta)};
            newPosition[index] = oldPosition[index] + offset;

            newPosition = oldPosition + glm::normalize(newPosition - oldPosition)* speedDelta;
        }
    }

    if (pNode == m_pCurrentNode || (pNode && !pNode->IsBlocked()))
    {
        m_pGameObject->SetPosition(newPosition.x, newPosition.y);
        m_pCurrentNode = pNode;
    }
}