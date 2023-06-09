#pragma once
#include <vector>
#include "Direction.h"
#include <glm/glm.hpp>

namespace dae
{
	class NavigationGrid;

	class NavigationNode final
	{
	public:
		NavigationNode(int column, int row, const glm::vec2 position, NavigationGrid* pGrid);
		virtual ~NavigationNode();

		NavigationNode* GetNeighbor(Direction direction) const;
		void SetNeighbor(Direction direction, NavigationNode* pNeighbor);

		int GetRow() const;
		int GetColumn() const;
		bool IsBlocked() const;
		void IsBlocked(bool isBlocked);

		NavigationGrid* GetGrid() const;
		glm::vec2 GetWorldPosition() const;
	private:
		const int m_Row;
		const int m_Column;

		std::vector<NavigationNode*> m_pNeighbors;
		glm::vec2 m_WorldPosition;

		NavigationGrid* m_pGrid;

		bool m_IsBlocked;
	};
}
