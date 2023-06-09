#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "NavigationNode.h"

namespace dae
{
	class NavigationNode;

	class NavigationGrid final
	{
	public:
		NavigationGrid();
		virtual ~NavigationGrid();

		NavigationNode* AddNode(int row, int column);
		NavigationNode* GetNode(int row, int column) const;
		NavigationNode* GetNode(const glm::vec2& position) const;
		NavigationNode* GetRandomNode() const;

		void SetNodeDimensions(int width, int height);
		int GetSmallerDimension() const;

		void Clear();
	private:
		std::vector<std::unique_ptr<NavigationNode>> m_pNodes;
		int m_NodeWidth;
		int m_NodeHeight;

		int m_MaxRow;
		int m_MaxColumn;
	};
}