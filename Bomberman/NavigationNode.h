#pragma once
#include <vector>
#include "Direction.h"
#include <glm/glm.hpp>

namespace dae
{
	class NavigationNode final
	{
	public:
		NavigationNode(int row, int column, const glm::vec2 position);

		NavigationNode* GetNeighbor(Direction direction) const;
		void SetNeighbor(Direction direction, NavigationNode* pNeighbor);

		int GetRow() const { return m_Row; };
		int GetColumn() const { return m_Column; };
		glm::vec2 GetWorldPosition() const;
	private:
		const int m_Row;
		const int m_Column;

		std::vector<NavigationNode*> m_pNeighbors{4};
		glm::vec2 m_WorldPosition{};
	};
}
