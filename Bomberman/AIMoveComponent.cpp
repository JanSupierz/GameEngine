#include "AIMoveComponent.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
#include "GameObject.h"
#include "UpdatePosition2DCommand.h"
#include <glm/glm.hpp>

dae::AIMoveComponent::AIMoveComponent(float speed, int priority)
	:Component{ priority }, m_Speed{ speed },
	m_pCurrentNode{ nullptr }
{
}

void dae::AIMoveComponent::Update()
{
	if (m_pCurrentNode)
	{
		NavigationNode* pNeighbor{ m_pCurrentNode->GetNeighbor(m_MoveDirection) };

		if (!pNeighbor || (pNeighbor && pNeighbor->IsBlocked()))
		{
			for(int counter{}; counter < 4; ++counter)
			{
				if (m_MoveDirection == Direction::end)
				{
					m_MoveDirection = Direction::begin;
				}
				else
				{
					m_MoveDirection = static_cast<Direction>(static_cast<int>(m_MoveDirection) + 1);
				}

				pNeighbor = m_pCurrentNode->GetNeighbor(m_MoveDirection);
				if (pNeighbor && !pNeighbor->IsBlocked()) break;
			} 
		}

		if (pNeighbor)
		{
			glm::vec2 vector{ pNeighbor->GetWorldPosition() - GetOwner()->GetWorldPosition() };

			m_pMoveCommand->SetValue(glm::normalize(vector));
			m_pMoveCommand->Execute();

			if (glm::length(vector) < 5.f)
			{
				m_pCurrentNode = pNeighbor;
			}
		}
	}
	else
	{
		m_pMoveCommand = std::move(std::make_unique<UpdatePosition2DCommand>(GetOwner(), m_Speed));
		m_pCurrentNode = NavigationGrid::GetInstance().GetNode(GetOwner()->GetWorldPosition());
	}
}