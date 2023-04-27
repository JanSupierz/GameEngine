#include "NavigationNode.h"

dae::NavigationNode::NavigationNode(int row, int column, const glm::vec2 position)
	:m_Row{ row }, m_Column{ column }, m_WorldPosition{ position }
{
}

dae::NavigationNode* dae::NavigationNode::GetNeighbor(Direction direction) const
{
	return m_pNeighbors[static_cast<int>(direction)];
}

void dae::NavigationNode::SetNeighbor(Direction direction, NavigationNode* pNeighbor)
{
	m_pNeighbors[static_cast<int>(direction)] = pNeighbor;
}

glm::vec2 dae::NavigationNode::GetWorldPosition() const
{
	return m_WorldPosition;
}
