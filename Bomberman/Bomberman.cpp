#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include <memory>

#include <SDL.h>
#include <iostream>

#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"

#include "GridMovementCommand.h"
#include "UpdatePosition1DCommand.h"
#include "UpdatePosition2DCommand.h"
#include "SetPositionCommand.h"

#include "RenderComponent.h"
#include "RotatorComponent.h"
#include "GameObject.h"
#include "Controller.h"
#include "Keyboard.h"

#include "PlayerComponent.h"
#include "LivesComponent.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "PlaceBombCommand.h"
#include "ScoreComponent.h"
#include "ScoresManager.h"
#include "StartInfoComponent.h"

//Events
#include "GainedPointEvent.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "BombExplodedEvent.h"

//Navigation
#include "NavigationGrid.h"
#include "AIComponent.h"
#include "AIWalkCommand.h"

//Json
#include <rapidjson.h>
#include <document.h>
#include <istreamwrapper.h>
#include <fstream >

#include "SpriteRenderComponent.h"
#include "BlockingComponent.h"

//Sound
#include "Audio.h"
#include "SDL_SoundSystem.h"
#include "LoggingSoundSystem.h"
#include "BombComponent.h"

//Collision
#include "ColliderComponent.h"
#include "EnemyComponent.h"

using namespace dae;

namespace Exercises
{
	void Drawing(Scene& scene);
	void Rotating(Scene& scene);
	void TrashTheCache(Scene& scene);
}

using rapidjson::Value;

std::shared_ptr<GameObject> CreatePlayer(const NavigationNode* const pNode,
	const std::string& name, const float playerSpeed, Scene& scene, const int spritePosX,
	const int spritePosY, const float infoDisplayOffsetX = 10.f, const float infoDisplayOffsetY = 20.f,
	const float infoDisplaySpacing = 20.f, const int spriteCellDimensions = 16, const float spriteCellScale = 2.f)
{
	//Player
	const auto pPlayerObject{ std::make_shared<GameObject>(-10) };

	const auto pCollider{ std::make_shared<ColliderComponent>(glm::vec2{32.f,32.f}) };
	pPlayerObject->AddComponent(pCollider);

	const int health{ 3 };
	const auto pPlayer{ std::make_shared<PlayerComponent>(pNode->GetWorldPosition(),name,health, pCollider->GetCollisionEvent()) };
	pPlayerObject->AddComponent(pPlayer);
	pPlayerObject->SetPosition(pNode->GetWorldPosition());

	const auto pPlayerRenderer{ std::make_shared<SpriteRenderComponent>(spritePosX,spritePosY,spriteCellDimensions,spriteCellDimensions,spriteCellScale) };
	pPlayerRenderer->SetTexture("BombermanSprites.png");
	pPlayerObject->AddComponent(pPlayerRenderer);

	scene.Add(pPlayerObject);

	//Health Display
	const auto pHealthDisplay{ std::make_shared<GameObject>() };
	pHealthDisplay->SetPosition(infoDisplayOffsetX, infoDisplayOffsetY);
	const auto pHealthRenderer{ std::make_shared<RenderComponent>() };
	pHealthDisplay->AddComponent(pHealthRenderer);

	constexpr int fontSize{ 20 };
	const auto pHealthText{ std::make_shared<TextComponent>(pHealthRenderer) };
	pHealthText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pHealthDisplay->AddComponent(pHealthText);

	const auto pLives{ std::make_shared<LivesComponent>(pPlayer.get(), pHealthText) };
	pHealthDisplay->AddComponent(pLives);

	scene.Add(pHealthDisplay);

	//Score Display 1
	const auto pScoreDisplay{ std::make_shared<GameObject>() };
	pScoreDisplay->SetPosition(infoDisplayOffsetX, infoDisplayOffsetY + infoDisplaySpacing);
	const auto pScoreRenderer{ std::make_shared<RenderComponent>() };
	pScoreDisplay->AddComponent(pScoreRenderer);

	const auto pScoreText{ std::make_shared<TextComponent>(pScoreRenderer) };
	pScoreText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pScoreDisplay->AddComponent(pScoreText);

	const auto pScore{ std::make_shared<ScoreComponent>(pPlayer, pScoreText) };
	pScoreDisplay->AddComponent(pScore);

	scene.Add(pScoreDisplay);

	//Controller
	auto& input = InputManager::GetInstance();

	const auto pController{ input.AddController() };
	pController->MapCommandToButton(Controller::ControllerButtons::ButtonA, std::make_unique<PlaceBombCommand>(pPlayerObject.get()), ButtonState::Down);

	pController->MapCommandToButton(Controller::ControllerButtons::DPadLeft, std::make_unique<GridMovementCommand>(pPlayerObject.get(), glm::vec2{ -playerSpeed,0.f }, pPlayer.get()), ButtonState::Pressed);
	pController->MapCommandToButton(Controller::ControllerButtons::DPadRight, std::make_unique<GridMovementCommand>(pPlayerObject.get(), glm::vec2{ playerSpeed,0.f }, pPlayer.get()), ButtonState::Pressed);
	pController->MapCommandToButton(Controller::ControllerButtons::DPadUp, std::make_unique<GridMovementCommand>(pPlayerObject.get(), glm::vec2{ 0.f,-playerSpeed }, pPlayer.get()), ButtonState::Pressed);
	pController->MapCommandToButton(Controller::ControllerButtons::DPadDown, std::make_unique<GridMovementCommand>(pPlayerObject.get(), glm::vec2{ 0.f,playerSpeed }, pPlayer.get()), ButtonState::Pressed);

	pController->MapCommandToThumbstick(Controller::ControllerThumbsticks::LeftThumbstick, std::make_unique<GridMovementCommand>(pPlayerObject.get(), glm::vec2{ playerSpeed,0.f }, pPlayer.get(), true));

	return pPlayerObject;
}

std::shared_ptr<GameObject> CreateEnemy(const NavigationNode* const pNode, DeathType enemyType, const float speed, Scene& scene,
	const int spritePosX, const int spritePosY, const int spriteCellDimensions = 16, const float spriteCellScale = 2.f)
{
	//Enemy
	const auto pEnemyObject{ std::make_shared<GameObject>(-10) };

	constexpr float margin{ 4.f };
	const float dimension{ static_cast<float>(NavigationGrid::GetInstance().GetSmallerDimension()) - margin };

	const auto pCollider{ std::make_shared<ColliderComponent>(glm::vec2{dimension, dimension}) };
	pEnemyObject->AddComponent(pCollider);

	const auto pEnemy{ std::make_shared<EnemyComponent>(enemyType, pCollider->GetCollisionEvent()) };
	pEnemyObject->AddComponent(pEnemy);
	pEnemyObject->SetPosition(pNode->GetWorldPosition());

	const auto pEnemyRenderer{ std::make_shared<SpriteRenderComponent>(spritePosX,spritePosY,spriteCellDimensions,spriteCellDimensions,spriteCellScale) };
	pEnemyRenderer->SetTexture("BombermanSprites.png");
	pEnemyObject->AddComponent(pEnemyRenderer);

	auto pAIWalkCommand{ std::make_unique<AIWalkCommand>(pEnemyObject.get(),speed) };

	const auto pAI{ std::make_shared<AIComponent>(std::move(pAIWalkCommand)) };
	pEnemyObject->AddComponent(pAI);

	scene.Add(pEnemyObject);

	return pEnemyObject;
}


void LoadReserved(const Value& cells, std::vector<std::tuple<int, int, std::string>>& reservedCubes)
{
	for (auto cubeIt{ cells.Begin() }; cubeIt != cells.End(); ++cubeIt)
	{
		const Value& node{ *cubeIt };
		int column{ node[0].GetInt() };

		const Value& rows{ node[1] };
		for (auto rowIt{ rows.Begin() }; rowIt != rows.End(); ++rowIt)
		{
			int row{ (*rowIt).GetInt() };

			reservedCubes.push_back(std::make_tuple(column, row, node[2].GetString()));
		}
	}
}

void LoadCells(const Value& cells, float cellSizeX, float cellSizeY, bool createNavigation = false)
{
	auto scene{ SceneManager::GetInstance().GetCurrentScene() };
	auto& grid{ NavigationGrid::GetInstance() };

	//Get sprite dimensions
	const int spritePosX{ cells[0][0].GetInt() };
	const int spritePosY{ cells[0][1].GetInt() };
	const int spriteSizeX{ cells[1][0].GetInt() };
	const int spriteSizeY{ cells[1][1].GetInt() };

	//Add cells to the scene
	for (auto cubeIt{ cells.Begin() + 2 }; cubeIt != cells.End(); ++cubeIt)
	{
		const Value& node{ *cubeIt };

		int column{ node[0].GetInt() };
		const Value& rows{ node[1] };
		for (auto rowIt{ rows.Begin() }; rowIt != rows.End(); ++rowIt)
		{
			int row{ (*rowIt).GetInt() };

			if (createNavigation)
			{
				grid.AddNode(row, column);
			}

			const auto pGameObject{ std::make_shared<GameObject>() };

			const auto pRenderComponent
			{
				std::make_shared<SpriteRenderComponent>
				(
				spritePosX * spriteSizeX,
				spritePosY * spriteSizeY,
				spriteSizeX,
				spriteSizeY,
				cellSizeX / spriteSizeY
				)
			};

			pRenderComponent->SetTexture("BombermanSprites.png");

			pGameObject->AddComponent(pRenderComponent);
			pGameObject->SetPosition(column * cellSizeX + cellSizeX * 0.5f, row * cellSizeY + cellSizeY * 0.5f);

			scene->Add(pGameObject);
		}
	}
}

bool IsReserved(const NavigationNode* const pNode, const std::vector<std::tuple<int, int, std::string>>& reservedCubes)
{
	int row{ pNode->GetRow() }, column{ pNode->GetColumn() };

	const auto it = std::find_if(reservedCubes.begin(), reservedCubes.end(), [&](const auto& tuple)
		{
			return std::get<0>(tuple) == column && std::get<1>(tuple) == row;
		});

	//Is reserved
	return it != reservedCubes.end();
}

void PlaceGameObjects(int maxNrBlocks, Scene& scene, const std::vector<std::tuple<int, int, std::string>>& reservedCubes)
{
	//Place blocking cubes
	std::vector<NavigationNode*> pNodesWithCubes{};
	pNodesWithCubes.reserve(maxNrBlocks);

	for (int index{}; index < maxNrBlocks; ++index)
	{
		NavigationNode* pNode{ NavigationGrid::GetInstance().GetRandomNode() };

		if (pNode && !pNode->IsBlocked() && !IsReserved(pNode, reservedCubes))
		{
			const auto pCubeObject{ std::make_shared<GameObject>(-10) };

			const auto pCube{ std::make_shared<BlockingComponent>(pNode) };
			pCubeObject->AddComponent(pCube);
			pCubeObject->SetPosition(pNode->GetWorldPosition());

			const auto pCubeRenderer{ std::make_shared<SpriteRenderComponent>(4 * 16,3 * 16,16,16,2.f) };
			pCubeRenderer->SetTexture("BombermanSprites.png");
			pCubeObject->AddComponent(pCubeRenderer);

			scene.Add(pCubeObject);

			pNodesWithCubes.push_back(pNode);
		}
	}

	//Place door
	NavigationNode* pNode{ pNodesWithCubes[rand() % pNodesWithCubes.size()] };

	const auto pDoorObject{ std::make_shared<GameObject>(-7) };

	const auto pDoorRenderer{ std::make_shared<SpriteRenderComponent>(11 * 16,3 * 16,16,16,2.f) };
	pDoorRenderer->SetTexture("BombermanSprites.png");
	pDoorObject->AddComponent(pDoorRenderer);

	pDoorObject->SetPosition(pNode->GetWorldPosition());

	scene.Add(pDoorObject);

	NavigationGrid& navigation{ NavigationGrid::GetInstance() };

	for (auto& tuple: reservedCubes)
	{
		std::string name{ std::get<2>(tuple) };

		if (name == "Balloom")
		{
			pNode = navigation.GetNode(std::get<0>(tuple), std::get<1>(tuple));

			if (pNode)
			{
				constexpr float balloomSpeed{ 100.f };
				CreateEnemy(pNode, DeathType::Balloom, balloomSpeed, scene, 0, 15 * 16);
			}
		}
	}
}

void LoadLevelFromFile(const std::string& fileName, std::vector<std::tuple<int, int, std::string>>& reservedCubes, Scene& scene)
{
	if (std::ifstream is{ ResourceManager::GetInstance().GetDataPath() + fileName })
	{
		float cellSizeX{ 100.f }, cellSizeY{ 100.f };

		rapidjson::IStreamWrapper isw{ is };
		rapidjson::Document document{};

		document.ParseStream(isw);

		if (document.HasMember("Dimensions"))
		{
			const Value& dimensions{ document["Dimensions"] };
			cellSizeX = dimensions[0].GetFloat();
			cellSizeY = dimensions[1].GetFloat();
		}

		if (document.HasMember("Navigation"))
		{
			auto& grid{ NavigationGrid::GetInstance() };
			grid.SetNodeDimensions(static_cast<int>(cellSizeX), static_cast<int>(cellSizeY));

			const Value& navigation{ document["Navigation"] };
			LoadCells(navigation, cellSizeX, cellSizeY, true);
		}

		if (document.HasMember("SolidCubes"))
		{
			const Value& navigation{ document["SolidCubes"] };
			LoadCells(navigation, cellSizeX, cellSizeY);
		}

		if (document.HasMember("ReservedPlaces"))
		{
			const Value& reservedPlaces{ document["ReservedPlaces"] };
			LoadReserved(reservedPlaces, reservedCubes);
		}

		if (document.HasMember("MaxNrCubes"))
		{
			const Value& maxNrCubes{ document["MaxNrCubes"] };
			PlaceGameObjects(maxNrCubes.GetInt(), scene, reservedCubes);
		}
	}
}

void load()
{
#if _DEBUG
	Audio::RegisterService(std::make_unique<LoggingSoundSystem>(std::make_unique<SDL_SoundSystem>()));
#else
	ServiceLocator<SoundSystem>::RegisterService(std::make_unique<SDL_SoundSystem>());
#endif

	//Load sounds
	int bombSoundId{};
	Audio::Get().AddSound("Explosion.wav", bombSoundId);
	Audio::Get().Preload(bombSoundId);
	BombComponent::SetExplosionSound(bombSoundId);

	int deathSound{};
	Audio::Get().AddSound("Death.wav", deathSound);
	PlayerComponent::SetDeathSound(deathSound);

	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& input = InputManager::GetInstance();
	auto& navigation = NavigationGrid::GetInstance();
	ScoresManager::GetInstance();

	//Create level
	std::vector<std::tuple<int, int, std::string>> reservedCubes{};
	LoadLevelFromFile("Bomberman.json", reservedCubes, scene);

	//Create players
	constexpr float speed{ 100.f };
	const auto pPlayerObject1{ CreatePlayer(navigation.GetNode(3, 1), "Player 1", speed, scene, 0, 0) };

	const auto pPlayer1{ pPlayerObject1->GetComponent<PlayerComponent>() };

	const auto pKeyboard{ input.GetKeyboard() };
	pKeyboard->MapCommandToButton(SDL_SCANCODE_LEFT, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ -speed,0.f }, pPlayer1.get()), ButtonState::Pressed);
	pKeyboard->MapCommandToButton(SDL_SCANCODE_RIGHT, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ speed,0.f }, pPlayer1.get()), ButtonState::Pressed);
	pKeyboard->MapCommandToButton(SDL_SCANCODE_UP, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,-speed }, pPlayer1.get()), ButtonState::Pressed);
	pKeyboard->MapCommandToButton(SDL_SCANCODE_DOWN, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,speed }, pPlayer1.get()), ButtonState::Pressed);

	pKeyboard->MapCommandToButton(SDL_SCANCODE_S, std::make_unique<PlaceBombCommand>(pPlayerObject1.get()), ButtonState::Down);
	//const auto pPlayerObject2{ CreatePlayer(navigation.GetNode(3, 1), "Player 2", speed, scene, 0, 15 * 16, 440.f) };

	//Start Info
	const auto pStartInfoDisplay{ std::make_shared<GameObject>() };
	const auto pStartInfo{ std::make_shared<StartInfoComponent>() };
	pStartInfoDisplay->AddComponent(pStartInfo);
	scene.Add(pStartInfoDisplay);
}

int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}

