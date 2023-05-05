#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Singleton.h"
#include "NavigationNode.h"

namespace dae
{
	class NavigationGrid final : public Singleton<NavigationGrid>
	{
	public:
		virtual ~NavigationGrid() = default;

		NavigationNode* AddNode(int row, int column);
		NavigationNode* GetNode(int row, int column) const;
		NavigationNode* GetNode(const glm::vec2& position) const;
		NavigationNode* GetRandomNode() const;

		void SetNodeDimensions(int width, int height);
		int GetSmallerDimension() const { return std::min(m_NodeWidth, m_NodeHeight); };
	private:
		friend class Singleton<NavigationGrid>;
		NavigationGrid() = default;

		std::vector<std::unique_ptr<NavigationNode>> m_pNodes{};
		int m_NodeWidth{ 32 };
		int m_NodeHeight{ 32 };

		int m_MaxRow{};
		int m_MaxColumn{};
	};
}