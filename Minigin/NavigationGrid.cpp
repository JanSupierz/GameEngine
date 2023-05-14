#include "NavigationGrid.h"
#include "NavigationNode.h"
#include "Direction.h"
#include <sstream>
#include "LoggingSystem.h"

dae::NavigationNode* dae::NavigationGrid::AddNode(int row, int column)
{
    m_MaxRow = std::max(m_MaxRow, row);
    m_MaxColumn = std::max(m_MaxColumn, column);

    glm::vec2 location{ column * m_NodeWidth + m_NodeWidth / 2.f, row * m_NodeHeight + m_NodeHeight / 2.f };

    {
        auto pTempNode{ GetNode(row,column) };

        if (pTempNode)
        {
            std::stringstream ss{};

            ss << "Node at location already exists: " << row << " " << column << '\n';
            ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
            return pTempNode;
        }
    }

    auto pNode{ std::make_unique<NavigationNode>(row, column, location) };

    // Check for neighbors in the four directions
    std::vector<std::pair<int, int>> directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    for (auto direction : directions)
    {   
        NavigationNode* pNeighbor{ GetNode(row + direction.first, column + direction.second) };

        if (pNeighbor)
        {
            Direction neighbor{};
            Direction current{};

            if (direction.second == 1)
            {
                neighbor = Direction::right;
                current = Direction::left;
            }
            else if (direction.second == -1)
            {
                neighbor = Direction::left;
                current = Direction::right;
            }
            else if (direction.first == 1)
            {
                neighbor = Direction::up;
                current = Direction::down;
            }
            else
            {
                neighbor = Direction::down;
                current = Direction::up;
            }

            pNode->SetNeighbor(neighbor, pNeighbor);
            pNeighbor->SetNeighbor(current, pNode.get());
        }
    }

    m_pNodes.push_back(std::move(pNode));

    return m_pNodes.back().get();
}

dae::NavigationNode* dae::NavigationGrid::GetNode(int row, int column) const
{
    for (const auto& pNode : m_pNodes)
    {
        if (pNode->GetRow() == row && pNode->GetColumn() == column)
        {
            return pNode.get();
        }
    }
    return nullptr;
}

dae::NavigationNode* dae::NavigationGrid::GetNode(const glm::vec2& position) const
{
    const int column{ static_cast<int>(position.x) / m_NodeWidth };
    const int row{ static_cast<int>(position.y) / m_NodeHeight };

    return GetNode(row, column);
}

dae::NavigationNode* dae::NavigationGrid::GetRandomNode() const
{   
    return GetNode(rand() % m_MaxRow + 1, rand() % m_MaxColumn + 1);
}

void dae::NavigationGrid::SetNodeDimensions(int width, int height)
{
    m_NodeWidth = width;
    m_NodeHeight = height;
}
