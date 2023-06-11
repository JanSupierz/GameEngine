#include "AIWalkCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
#include <Algorithm>
#include <random>
#include "BombermanManager.h"
#include "Scene.h"
#include "Logger.h"

using namespace dae;

AIWalkCommand::AIWalkCommand(GameObject* pGameObject, float speed, bool isSmart)
	: m_pGameObject(pGameObject),
	m_Speed{ speed },
	m_pSceneManager(&SceneManager::GetInstance()),
	m_pGrid{ m_pSceneManager->GetCurrentScene()->GetGrid() },
	m_IsSmart{ isSmart }
{
	m_DirectionsToCheck.resize(4);
	m_DirectionsToCheck[0] = Direction::up;
	m_DirectionsToCheck[1] = Direction::down;
	m_DirectionsToCheck[2] = Direction::left;
	m_DirectionsToCheck[3] = Direction::right;
};

void dae::AIWalkCommand::SetTarget(GameObject* pTarget)
{
	m_pTarget = pTarget;
}

void dae::AIWalkCommand::Execute()
{
    if (!m_pGameObject) return;

	glm::vec2 worldPos{ m_pGameObject->GetWorldPosition() };
    const float speedDelta{ m_Speed * m_pSceneManager->GetDeltaTime() };

	if (m_pCurrentNode)
	{
		NavigationNode* pNeighbor{ nullptr };

		if (m_IsSmart)
		{
			auto pObjects{ BombermanManager::GetInstance().GetPlayerObjects() };

			auto pOldTarget{ m_pTarget };
			m_pTarget = nullptr;

			for (const auto& pObject : pObjects)
			{
				if (glm::length(pObject->GetWorldPosition() - worldPos) < 100)
				{
					m_pTarget = pObject;

					if (m_pTarget == pOldTarget)
					{
						break;
					}
				}
			}

			//Seek target
			if (m_pTarget)
			{
				glm::vec2 targetPos{ m_pTarget->GetWorldPosition() };
				NavigationNode* pTargetNode{ m_pGrid->GetNode(targetPos) };

				const int columnDiff{ m_pCurrentNode->GetColumn() - pTargetNode->GetColumn() };
				const int rowDiff{ m_pCurrentNode->GetRow() - pTargetNode->GetRow() };

				if (abs(columnDiff) > abs(rowDiff))
				{
					Logger::Get().Log("horizon");
					pNeighbor = m_pCurrentNode->GetNeighbor(columnDiff > 0 ? Direction::left : Direction::right);
				}
				else
				{
					pNeighbor = m_pCurrentNode->GetNeighbor(rowDiff < 0 ? Direction::up : Direction::down);
				}
			}
		}

		//Wander around
		if (!pNeighbor || (pNeighbor && pNeighbor->IsBlocked()))
		{
			//Get next node
			pNeighbor = m_pCurrentNode->GetNeighbor(m_Direction);

			//Node not valid -> Check other nodes
			if (!pNeighbor || (pNeighbor && pNeighbor->IsBlocked()))
			{
				static auto rd{ std::random_device{} };
				std::shuffle(begin(m_DirectionsToCheck), end(m_DirectionsToCheck), rd);

				for (Direction direction : m_DirectionsToCheck)
				{
					//Get first free neighbor
					pNeighbor = m_pCurrentNode->GetNeighbor(direction);
					if (pNeighbor && !pNeighbor->IsBlocked())
					{
						m_Direction = direction;
						break;
					}
				}
			}
		}

		if (pNeighbor && !pNeighbor->IsBlocked())
		{
			glm::vec2 vector{ pNeighbor->GetWorldPosition() - worldPos };

			if (glm::length(vector) < 2.f)
			{
				m_pCurrentNode = pNeighbor;
			}

			m_pGameObject->SetPosition(worldPos + glm::normalize(vector) * speedDelta);
		}
	}
	else
	{
		m_pCurrentNode = m_pGrid->GetNode(worldPos);
	}
}