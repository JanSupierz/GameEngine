#include "NavigationGrid.h"
#include "NavigationNode.h"
#include "Direction.h"
#include <iostream>

void dae::NavigationGrid::AddNode(int row, int column)
{
    glm::vec2 location{ column * m_NodeWidth + m_NodeWidth / 2.f, row * m_NodeHeight + m_NodeHeight / 2.f };

    if (GetNode(row, column))
    {
        std::cout << "Node at location already exists: " << row << " " << column << '\n';
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

            if (direction.first == 1)
            {
                neighbor = Direction::down;
                current = Direction::up;
            }
            else if(direction.first == -1)
            {
                neighbor = Direction::up;
                current = Direction::down;
            }
            else if (direction.second == 1)
            {
                neighbor = Direction::right;
                current = Direction::left;
            }
            else
            {
                neighbor = Direction::left;
                current = Direction::right;
            }

            pNode->SetNeighbor(neighbor, pNeighbor);
            pNeighbor->SetNeighbor(current, pNode.get());
        }
    }

    m_pNodes.push_back(std::move(pNode));
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
    const int row{ static_cast<int>(position.x) / m_NodeHeight };
    const int column{ static_cast<int>(position.y) / m_NodeWidth };

    return GetNode(row, column);
}

void dae::NavigationGrid::SetNodeDimensions(int width, int height)
{
    m_NodeWidth = width;
    m_NodeHeight = height;
}
