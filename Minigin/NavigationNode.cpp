#include "NavigationNode.h"
#include "NavigationGrid.h"

dae::NavigationNode::NavigationNode(int row, int column, const glm::vec2 position, NavigationGrid* pGrid)
	:m_Row{ row }, m_Column{ column }, m_WorldPosition{ position }, 
	 m_pNeighbors{ 4 }, m_IsBlocked{ false }, m_pGrid{ pGrid }
{
}

dae::NavigationNode::~NavigationNode()
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

dae::NavigationGrid* dae::NavigationNode::GetGrid() const
{
	return m_pGrid;
}

glm::vec2 dae::NavigationNode::GetWorldPosition() const
{
	return m_WorldPosition;
}

int dae::NavigationNode::GetRow() const
{
	return m_Row;
}

int dae::NavigationNode::GetColumn() const
{
	return m_Column;
}

bool dae::NavigationNode::IsBlocked() const
{
	return m_IsBlocked;
}

void dae::NavigationNode::IsBlocked(bool isBlocked)
{
	m_IsBlocked = isBlocked;
}
