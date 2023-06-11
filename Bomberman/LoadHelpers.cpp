#include "LoadHelpers.h"
#include "Prefabs.h"
#include "ResourceManager.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
#include "SceneManager.h"
#include "Scene.h"

//Json
#include <document.h>
#include <istreamwrapper.h>
#include <fstream>

//Extra
#include "EnemyComponent.h"

#include <rapidjson.h>
#include "Logger.h"

void LoadCells(const rapidjson::Value& cells, float cellSizeX, float cellSizeY, bool createNavigation)
{
	auto& scene{ *dae::SceneManager::GetInstance().GetCurrentScene() };
	auto pGrid{ scene.GetGrid() };

	//Get sprite dimensions
	const int spritePosX{ cells[0][0].GetInt() };
	const int spritePosY{ cells[0][1].GetInt() };
	const int spriteSizeX{ cells[1][0].GetInt() };
	const int spriteSizeY{ cells[1][1].GetInt() };

	//Add cells to the scene
	for (auto cubeIt{ cells.Begin() + 2 }; cubeIt != cells.End(); ++cubeIt)
	{
		const rapidjson::Value& node{ *cubeIt };

		int column{ node[0].GetInt() };
		const rapidjson::Value& rows{ node[1] };
		for (auto rowIt{ rows.Begin() }; rowIt != rows.End(); ++rowIt)
		{
			int row{ (*rowIt).GetInt() };

			if (createNavigation)
			{
				pGrid->AddNode(row, column);
			}

			dae::CreateCube(scene, spritePosX * spriteSizeX, spritePosY * spriteSizeY, spriteSizeX, cellSizeX / spriteSizeX, { cellSizeX * (column + 0.5f), cellSizeY * (row + 0.5f) });
		}
	}
}

void LoadReserved(const rapidjson::Value& cells, std::vector<std::tuple<int, int, std::string>>& reservedCubes)
{
	for (auto cubeIt{ cells.Begin() }; cubeIt != cells.End(); ++cubeIt)
	{
		const rapidjson::Value& node{ *cubeIt };
		int column{ node[0].GetInt() };

		const rapidjson::Value& rows{ node[1] };
		for (auto rowIt{ rows.Begin() }; rowIt != rows.End(); ++rowIt)
		{
			int row{ (*rowIt).GetInt() };

			reservedCubes.push_back(std::make_tuple(column, row, node[2].GetString()));
		}
	}
}

void dae::LoadLevelFromFile(const std::string& fileName, std::vector<std::tuple<int, int, std::string>>& reservedCubes, Scene& scene)
{
	if (std::ifstream is{ ResourceManager::GetInstance().GetDataPath() + fileName })
	{
		float cellSizeX{ 100.f }, cellSizeY{ 100.f };

		rapidjson::IStreamWrapper isw{ is };
		rapidjson::Document document{};

		document.ParseStream(isw);

		if (document.HasMember("Dimensions"))
		{
			const rapidjson::Value& dimensions{ document["Dimensions"] };
			cellSizeX = dimensions[0].GetFloat();
			cellSizeY = dimensions[1].GetFloat();
		}

		if (document.HasMember("Navigation"))
		{
			auto& grid{ *scene.GetGrid() };
			grid.SetNodeDimensions(static_cast<int>(cellSizeX), static_cast<int>(cellSizeY));

			const rapidjson::Value& navigation{ document["Navigation"] };
			LoadCells(navigation, cellSizeX, cellSizeY, true);
		}

		if (document.HasMember("SolidCubes"))
		{
			const rapidjson::Value& navigation{ document["SolidCubes"] };
			LoadCells(navigation, cellSizeX, cellSizeY, false);
		}

		if (document.HasMember("ReservedPlaces"))
		{
			const rapidjson::Value& reservedPlaces{ document["ReservedPlaces"] };
			LoadReserved(reservedPlaces, reservedCubes);
		}

		if (document.HasMember("MaxNrCubes"))
		{
			const rapidjson::Value& maxNrCubes{ document["MaxNrCubes"] };
			PlaceGameObjects(maxNrCubes.GetInt(), scene, reservedCubes);
		}
	}
}

bool dae::IsReserved(const NavigationNode* const pNode, const std::vector<std::tuple<int, int, std::string>>& reservedCubes)
{
	int row{ pNode->GetRow() }, column{ pNode->GetColumn() };

	const auto it = std::find_if(reservedCubes.begin(), reservedCubes.end(), [&](const auto& tuple)
		{
			return std::get<0>(tuple) == column && std::get<1>(tuple) == row;
		});

	//Is reserved
	return it != reservedCubes.end();
}

void dae::PlaceGameObjects(int maxNrBlocks, Scene& scene, const std::vector<std::tuple<int, int, std::string>>& reservedCubes)
{
	//Place blocking cubes
	std::vector<NavigationNode*> pNodesWithCubes{};
	pNodesWithCubes.reserve(maxNrBlocks);

	for (int index{}; index < maxNrBlocks; ++index)
	{
		NavigationNode* pNode{ scene.GetGrid()->GetRandomNode() };

		if (pNode && !pNode->IsBlocked() && !IsReserved(pNode, reservedCubes))
		{
			CreateBlockingCube(scene, pNode);
			pNodesWithCubes.push_back(pNode);
		}
	}

	//Place door
	NavigationNode* pNode{ pNodesWithCubes[rand() % pNodesWithCubes.size()] };
	CreateDoor(scene, pNode);

	NavigationGrid& navigation{ *(scene.GetGrid()) };

	for (auto& tuple : reservedCubes)
	{
		pNode = navigation.GetNode(std::get<1>(tuple), std::get<0>(tuple));

		if (pNode)
		{
			std::string name{ std::get<2>(tuple) };

			if (name == "Balloom")
			{
				constexpr float balloomSpeed{ 50.f };
				CreateEnemy(pNode, DeathType::Balloom, balloomSpeed, scene, 0, 15 * 16);
			}
			else if (name == "Oneal")
			{
				constexpr float speed{ 80.f };
				CreateEnemy(pNode, DeathType::Oneal, speed, scene, 0, 16 * 16);
			}
			else if (name == "Doll")
			{
				constexpr float speed{ 80.f };
				CreateEnemy(pNode, DeathType::Doll, speed, scene, 0, 17 * 16);
			}
			else if (name == "Minvo")
			{
				constexpr float speed{ 100.f };
				CreateEnemy(pNode, DeathType::Minvo, speed, scene, 0, 18 * 16);
			}
		}
		else
		{
			Logger::Get().Log("Node: " + std::to_string(std::get<0>(tuple)) + ' ' + std::to_string(std::get<1>(tuple)) + " doesn't exist");
		}
	}
}
