#include "AIComponent.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
#include "GameObject.h"
#include "AIWalkCommand.h"
#include <glm/glm.hpp>

dae::AIComponent::AIComponent(std::unique_ptr<AIWalkCommand>&& pMoveCommand, int priority)
	:Component{ priority }, m_pMoveCommand{ std::move(pMoveCommand) }
{
}

void dae::AIComponent::Update()
{
	m_pMoveCommand->Execute();
}