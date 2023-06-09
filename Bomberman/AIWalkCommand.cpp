#include "AIWalkCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
#include <Algorithm>
#include <random>
#include "Scene.h"

using namespace dae;

AIWalkCommand::AIWalkCommand(GameObject* pGameObject, float speed)
	: m_pGameObject(pGameObject),
	m_Speed{ speed },
	m_pSceneManager(&SceneManager::GetInstance()),
	m_pGrid{ m_pSceneManager->GetCurrentScene()->GetGrid() }
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

		//Seek target
		if (m_pTarget)
		{
			glm::vec2 targetPos{ m_pTarget->GetWorldPosition() };
			glm::vec2 targetDirection{ targetPos - worldPos };

			bool horizontal{ abs(targetDirection.x) > abs(targetDirection.y) };

			if (horizontal)
			{
				if (targetDirection.x > 0.f)
				{
					pNeighbor = m_pCurrentNode->GetNeighbor(Direction::right);
				}
				else
				{
					pNeighbor = m_pCurrentNode->GetNeighbor(Direction::left);
				}
			}
			else
			{
				if (targetDirection.y > 0.f)
				{
					pNeighbor = m_pCurrentNode->GetNeighbor(Direction::up);
				}
				else
				{
					pNeighbor = m_pCurrentNode->GetNeighbor(Direction::down);
				}
			}

			if (!pNeighbor || (pNeighbor && pNeighbor->IsBlocked()))
			{
				if (!horizontal)
				{
					if (targetDirection.x > 0.f)
					{
						pNeighbor = m_pCurrentNode->GetNeighbor(Direction::right);
					}
					else
					{
						pNeighbor = m_pCurrentNode->GetNeighbor(Direction::left);
					}
				}
				else
				{
					if (targetDirection.y > 0.f)
					{
						pNeighbor = m_pCurrentNode->GetNeighbor(Direction::up);
					}
					else
					{
						pNeighbor = m_pCurrentNode->GetNeighbor(Direction::down);
					}
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

			if (glm::length(vector) < 5.f)
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