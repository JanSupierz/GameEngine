#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "NavigationNode.h"

namespace dae
{
	class NavigationGrid final : public Singleton<NavigationGrid>
	{
	public:
		virtual ~NavigationGrid() = default;

		void AddNode(int row, int column);
		NavigationNode* GetNode(int row, int column) const;
		NavigationNode* GetNode(const glm::vec2& position) const;

		void SetNodeDimensions(int width, int height);
	private:
		friend class Singleton<NavigationGrid>;
		NavigationGrid() = default;

		std::vector<std::unique_ptr<NavigationNode>> m_pNodes{};
		int m_NodeWidth{ 100 };
		int m_NodeHeight{ 100 };
	};
}