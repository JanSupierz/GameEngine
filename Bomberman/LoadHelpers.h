#pragma once
#include <string>
#include <vector>
#include "Scene.h"

namespace dae
{
	class NavigationNode;

	void LoadLevelFromFile(const std::string& fileName, std::vector<std::tuple<int, int, std::string>>& reservedCubes, Scene& scene);

	bool IsReserved(const NavigationNode* const pNode, const std::vector<std::tuple<int, int, std::string>>& reservedCubes);
	void PlaceGameObjects(int maxNrBlocks, Scene& scene, const std::vector<std::tuple<int, int, std::string>>& reservedCubes);
}
