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

#include "GainedPointEvent.h"
#include "PlayerDiedEvent.h"
#include "EventManager.h"
#include "BombExplodedEvent.h"
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
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "SDL_SoundSystem.h"
#include "LoggingSoundSystem.h"
#include "BombComponent.h"

using namespace dae;

namespace Exercises
{
	void Drawing(Scene& scene);
	void Rotating(Scene& scene);
	void TrashTheCache(Scene& scene);
}

using rapidjson::Value;

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

void LoadLevelFromFile(const std::string& fileName)
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
	}
}

void PlaceCubes(int maxNrBlocks, Scene& scene)
{
	int nrBlocks{};

	while (nrBlocks < maxNrBlocks)
	{
		NavigationNode* pNode{ NavigationGrid::GetInstance().GetRandomNode() };

		if (pNode && !pNode->IsBlocked())
		{
			++nrBlocks;

			const auto pCubeObject{ std::make_shared<GameObject>(-10) };

			const auto pCube{ std::make_shared<BlockingComponent>(pNode) };
			pCubeObject->AddComponent(pCube);
			pCubeObject->SetPosition(pNode->GetWorldPosition());

			const auto pCubeRenderer{ std::make_shared<SpriteRenderComponent>(4 * 16,3 * 16,16,16,2.f) };
			pCubeRenderer->SetTexture("BombermanSprites.png");
			pCubeObject->AddComponent(pCubeRenderer);

			scene.Add(pCubeObject);
		}
	}
}

void load()
{
#if _DEBUG
	ServiceLocator<SoundSystem>::RegisterService(std::make_unique <LoggingSoundSystem>(std::make_unique<SDL_SoundSystem>()));
#else
	ServiceLocator<SoundSystem>::RegisterService(std::make_unique<SDL_SoundSystem>());
#endif
	int bombSoundId{};
	ServiceLocator<SoundSystem>::GetService().AddSound("Explosion.wav", bombSoundId);
	ServiceLocator<SoundSystem>::GetService().Preload(bombSoundId);
	BombComponent::SetExplosionSound(bombSoundId);

	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& input = InputManager::GetInstance();
	ScoresManager::GetInstance();

	LoadLevelFromFile("Bomberman.json");
	PlaceCubes(20, scene);



	constexpr int fontSize{ 20 };

	//Player 1
	const auto pPlayerObject1{ std::make_shared<GameObject>(-10) };

	const auto pPlayer1{ std::make_shared<PlayerComponent>("Player 1",100) };
	pPlayerObject1->AddComponent(pPlayer1);
	pPlayerObject1->SetPosition(200.f, 200.f);

	const auto pPlayerRenderer1{ std::make_shared<SpriteRenderComponent>(0,0,16,16,2.f) };
	pPlayerRenderer1->SetTexture("BombermanSprites.png");
	pPlayerObject1->AddComponent(pPlayerRenderer1);

	scene.Add(pPlayerObject1);

	//Health Display 1
	const auto pHealthDisplay1{ std::make_shared<GameObject>() };
	pHealthDisplay1->SetPosition(10.f, 20.f);
	const auto pHealthRenderer1{ std::make_shared<RenderComponent>() };
	pHealthDisplay1->AddComponent(pHealthRenderer1);

	const auto pHealthText1{ std::make_shared<TextComponent>(pHealthRenderer1) };
	pHealthText1->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pHealthDisplay1->AddComponent(pHealthText1);

	const auto pLives1{ std::make_shared<LivesComponent>(pPlayer1.get(), pHealthText1) };
	pHealthDisplay1->AddComponent(pLives1);

	scene.Add(pHealthDisplay1);

	//Score Display 1
	const auto pScoreDisplay1{ std::make_shared<GameObject>() };
	pScoreDisplay1->SetPosition(10.f, 40.f);
	const auto pScoreRenderer1{ std::make_shared<RenderComponent>() };
	pScoreDisplay1->AddComponent(pScoreRenderer1);

	const auto pScoreText1{ std::make_shared<TextComponent>(pScoreRenderer1) };
	pScoreText1->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pScoreDisplay1->AddComponent(pScoreText1);

	const auto pScore1{ std::make_shared<ScoreComponent>(pPlayer1, pScoreText1) };
	pScoreDisplay1->AddComponent(pScore1);

	scene.Add(pScoreDisplay1);

	//Controller 1
	const auto pController1{ input.AddController() };
	pController1->MapCommandToButton(Controller::ControllerButtons::ButtonA, std::make_unique<PlaceBombCommand>(pPlayerObject1.get()), ButtonState::Down);

	constexpr float speed{ 100.f };

	pController1->MapCommandToButton(Controller::ControllerButtons::DPadLeft, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ -speed,0.f }, pPlayer1.get()), ButtonState::Pressed);
	pController1->MapCommandToButton(Controller::ControllerButtons::DPadRight, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ speed,0.f }, pPlayer1.get()), ButtonState::Pressed);
	pController1->MapCommandToButton(Controller::ControllerButtons::DPadUp, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,-speed }, pPlayer1.get()), ButtonState::Pressed);
	pController1->MapCommandToButton(Controller::ControllerButtons::DPadDown, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,speed }, pPlayer1.get()), ButtonState::Pressed);

	//Player 2
	const auto pPlayerObject2{ std::make_shared<GameObject>(-10) };

	auto pAIWalkCommand{ std::make_unique<AIWalkCommand>(pPlayerObject2.get(),100.f) };
	//pAIWalkCommand->SetTarget(pPlayerObject1.get());
	const auto pPlayer2{ std::make_shared<PlayerComponent>("Player 2",100) };
	pPlayerObject2->AddComponent(pPlayer2);

	const auto pAI{ std::make_shared<AIComponent>(std::move(pAIWalkCommand))};
	pPlayerObject2->AddComponent(pAI);
	pPlayerObject2->SetPosition(50.f, 50.f);  


	const auto pPlayerRenderer2{ std::make_shared<SpriteRenderComponent>(0,15 * 16,16,16,2.f) };
	pPlayerRenderer2->SetTexture("BombermanSprites.png");
	pPlayerObject2->AddComponent(pPlayerRenderer2);

	scene.Add(pPlayerObject2);

	//Health Display 2
	const auto pHealthDisplay2{ std::make_shared<GameObject>() };
	pHealthDisplay2->SetPosition(400.f, 20.f);
	const auto pHealthRenderer2{ std::make_shared<RenderComponent>() };
	pHealthDisplay2->AddComponent(pHealthRenderer2);

	const auto pHealthText2{ std::make_shared<TextComponent>(pHealthRenderer2) };
	pHealthText2->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pHealthDisplay2->AddComponent(pHealthText2);

	const auto pLives2{ std::make_shared<LivesComponent>(pPlayer2.get(), pHealthText2) };
	pHealthDisplay2->AddComponent(pLives2);

	scene.Add(pHealthDisplay2);

	//Score Display 2
	const auto pScoreDisplay2{ std::make_shared<GameObject>() };
	pScoreDisplay2->SetPosition(400.f, 40.f);
	const auto pScoreRenderer2{ std::make_shared<RenderComponent>() };
	pScoreDisplay2->AddComponent(pScoreRenderer2);

	const auto pScoreText2{ std::make_shared<TextComponent>(pScoreRenderer2) };
	pScoreText2->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pScoreDisplay2->AddComponent(pScoreText2);

	const auto pScore2{ std::make_shared<ScoreComponent>(pPlayer2, pScoreText2) };
	pScoreDisplay2->AddComponent(pScore2);

	scene.Add(pScoreDisplay2);

	//Keyboard 1
	const auto pKeyboard1{ input.GetKeyboard() };
	pKeyboard1->MapCommandToButton(SDL_SCANCODE_S, std::make_unique<PlaceBombCommand>(pPlayerObject2.get()), ButtonState::Down);

	pKeyboard1->MapCommandToButton(SDL_SCANCODE_LEFT, std::make_unique<GridMovementCommand>(pPlayerObject2.get(), glm::vec2{ -speed,0.f }, pPlayer2.get()), ButtonState::Pressed);
	pKeyboard1->MapCommandToButton(SDL_SCANCODE_RIGHT, std::make_unique<GridMovementCommand>(pPlayerObject2.get(), glm::vec2{ speed,0.f }, pPlayer2.get()), ButtonState::Pressed);
	pKeyboard1->MapCommandToButton(SDL_SCANCODE_UP, std::make_unique<GridMovementCommand>(pPlayerObject2.get(), glm::vec2{ 0.f,-speed }, pPlayer2.get()), ButtonState::Pressed);
	pKeyboard1->MapCommandToButton(SDL_SCANCODE_DOWN, std::make_unique<GridMovementCommand>(pPlayerObject2.get(), glm::vec2{ 0.f,speed }, pPlayer2.get()), ButtonState::Pressed);

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

