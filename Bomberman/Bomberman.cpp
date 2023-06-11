#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <thread>
#include <chrono>


#include "Prefabs.h"
#include "LoadHelpers.h"

#include "Minigin.h"
#include <memory>

#include <SDL.h>
#include <iostream>

#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"

#include "RenderComponent.h"
#include "RotatorComponent.h"
#include "GameObject.h"
#include "Controller.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ClickCommand.h"

#include "PlayerComponent.h"
#include "LivesComponent.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "PlaceBombCommand.h"
#include "ScoreComponent.h"
#include "BombermanManager.h"
#include "StartInfoComponent.h"
#include "LevelLoadComponent.h"
#include "PowerUp.h"
#include "NavigationGrid.h"

//Commands
#include "GridMovementCommand.h"

//Events
#include "HUDEvent.h"
#include "DeathEvent.h"
#include "EventManager.h"
#include "BombExplodedEvent.h"

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

//Camera
#include "CameraComponent.h"
#include "CameraTargetComponent.h"
#include "Renderer.h"

//Scene switch
#include "NextSceneCommand.h"

//HighScores
#include <iostream>
#include <fstream>

using namespace dae;

namespace Exercises
{
	void Drawing(Scene& scene);
	void Rotating(Scene& scene);
	void TrashTheCache(Scene& scene);
}

static int s_GameSoundId{};

void LoadLevel()
{
	//Clear inputs
	auto& input = InputManager::GetInstance();
	input.ClearCommands();

	Audio::Get().Play(s_GameSoundId, 1.f, -1);

	auto pScene{ SceneManager::GetInstance().GetCurrentScene() };
	auto& navigation = *pScene->GetGrid();

	auto& manager{ BombermanManager::GetInstance() };

	//Create level
	std::vector<std::tuple<int, int, std::string>> reservedCubes{};
	LoadLevelFromFile(manager.GetLevelPath(), reservedCubes, *pScene);

	//Set Camera
	const auto pCamera{ pScene->Add(std::make_shared<GameObject>()) };
	const auto pCameraComponent{ std::make_shared<CameraComponent>(300.f) };
	pCamera->AddComponent(pCameraComponent);

	glm::vec2 size{ Renderer::GetInstance().GetWindowSize() };
	pCameraComponent->SetBounds(glm::vec2{}, glm::vec2{ size.x * 2 - 32.f,size.y });
	pScene->SetCamera(pCamera.get());

	//Create players
	constexpr float speed{ 100.f };
	const auto pPlayerObject1{ CreatePlayer(0,navigation.GetNode(3, 1), speed, *pScene, 0, 0) };

	const auto pPlayer1{ pPlayerObject1->GetComponent<PlayerComponent>() };

	//Input
	const auto pKeyboard{ input.GetKeyboard() };

	pKeyboard->MapCommandToButton(SDL_SCANCODE_LEFT, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ -speed,0.f }, pPlayer1.get()), ButtonState::Pressed);
	pKeyboard->MapCommandToButton(SDL_SCANCODE_RIGHT, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ speed,0.f }, pPlayer1.get()), ButtonState::Pressed);
	pKeyboard->MapCommandToButton(SDL_SCANCODE_UP, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,-speed }, pPlayer1.get()), ButtonState::Pressed);
	pKeyboard->MapCommandToButton(SDL_SCANCODE_DOWN, std::make_unique<GridMovementCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,speed }, pPlayer1.get()), ButtonState::Pressed);

	pKeyboard->MapCommandToButton(SDL_SCANCODE_S, std::make_unique<PlaceBombCommand>(pPlayerObject1.get()), ButtonState::Down);

	input.GetKeyboard()->MapCommandToButton(SDL_SCANCODE_F1, std::make_unique<NextSceneCommand>(), ButtonState::Down);

	if (manager.GetMode() != GameMode::SinglePlayer)
	{
		int spriteOffsetY{ manager.GetMode() == GameMode::Versus ? 15 * 16 : 0 };
		const auto pPlayerObject2{ CreatePlayer(1,navigation.GetNode(13, 1), speed, *pScene, 0, spriteOffsetY, 440.f) };
	}

	BombermanManager::GetInstance().RefreshHUD();
}

void LoadLoadingScene()
{
	//Clear inputs
	auto& input = InputManager::GetInstance();
	input.ClearCommands();

	auto pScene{ SceneManager::GetInstance().GetCurrentScene() };
	auto& manager{ BombermanManager::GetInstance() };

	//Set Camera
	const auto pCamera{ pScene->Add(std::make_shared<GameObject>()) };
	const auto pCameraComponent{ std::make_shared<CameraComponent>(300.f) };
	pCamera->AddComponent(pCameraComponent);

	glm::vec2 size{ Renderer::GetInstance().GetWindowSize() };
	pCameraComponent->SetBounds(glm::vec2{}, glm::vec2{ size.x * 2 - 32.f,size.y });
	pScene->SetCamera(pCamera.get());

	const auto pScreen{ CreateLoadingScreen(*pScene, static_cast<int>(size.x), static_cast<int>(size.y), "Stage " + std::to_string(manager.GetCurrentLevelIndex() + 1), 30, -100) };
}

Scene& InitGameScene()
{
	//Create a new scene
	auto pScene = SceneManager::GetInstance().GetScene("LoadingScene");

	//Destroy old scene
	SceneManager::GetInstance().GetScene("MenuScene")->DestroyAll();

	return *pScene;
}

void LoadScoreScene()
{
	auto& input = InputManager::GetInstance();
	input.ClearCommands();

	auto pCurrent{ SceneManager::GetInstance().GetCurrentScene() };
	auto& manager{ BombermanManager::GetInstance() };

	const glm::vec2 size{ Renderer::GetInstance().GetWindowSize() };
	constexpr int titleFontSize{ 50 };
	CreateText(*pCurrent, glm::vec2{ size.x / 2 - 120.f, size.y / 5.f }, "High Scores", titleFontSize);

	//Load scores from file
	std::ifstream scoresFile{ "Scores.txt" };
	std::string line{};
	glm::vec2 position{ size.x / 2.f, size.y / 3.f };

	if (scoresFile.is_open())
	{
		while (std::getline(scoresFile, line))
		{
			//Read the name and the score
			std::istringstream buffer(line);

			std::string name;
			int score;

			if (std::getline(buffer, name, '-'))
			{
				if (std::getline(buffer, line))
				{
					score = std::stoi(line);

					manager.AddScore(name, score);
				}
			}
		}
	}

	auto scores{ manager.GetTopScores(5) };
	
	for (auto& scorePair : scores)
	{
		constexpr int fontSize{ 30 };
		CreateText(*pCurrent, position, scorePair.first + " - " + std::to_string(scorePair.second), fontSize);
	}

	input.
}

void LoadMenu()
{
	auto& input = InputManager::GetInstance();
	input.ClearCommands();

	auto pScene{ SceneManager::GetInstance().GetCurrentScene() };
	auto windowSize{ Renderer::GetInstance().GetWindowSize() };
	
	constexpr int fontSize{ 20 };
	glm::vec2 buttonPos{ 2.f * windowSize.x / 5.f, 0.6f * windowSize.y };
	constexpr float offsetY{ 50.f };
	
	dae::CreateButton(*pScene, buttonPos, "Play", fontSize, []()
		{
			auto& scene{ InitGameScene() };

			BombermanManager::GetInstance().AddPlayer("Player 1");

			scene.Load();
		});

	buttonPos.y += offsetY;

	dae::CreateButton(*pScene, buttonPos, "Play Co-op", fontSize, []()
		{
			auto& scene{ InitGameScene() };

			auto& manager{ BombermanManager::GetInstance() };
			manager.AddPlayer("Player 1");
			manager.AddPlayer("Player 2");
			manager.SetGameMode(GameMode::Coop);

			scene.Load();
		});

	buttonPos.y += offsetY;
	dae::CreateButton(*pScene, buttonPos, "Play Versus", fontSize, []()
		{
			auto& scene{ InitGameScene() };

			auto& manager{ BombermanManager::GetInstance() };
			manager.AddPlayer("Player 1");
			manager.AddPlayer("Player 2");
			manager.SetGameMode(GameMode::Versus);

			scene.Load();
		});

	buttonPos.y += offsetY;
	dae::CreateButton(*pScene, buttonPos, "Score Board", fontSize, []()
		{
			SceneManager::GetInstance().GetScene("ScoreScene")->Load();
			SceneManager::GetInstance().GetScene("MenuScene")->DestroyAll();
		});
	
	const glm::vec2 imagePos{ windowSize.x / 2.f, 0.3f * windowSize.y };
	dae::CreateImage(*pScene, imagePos, 379, 232, 1.f);

	input.GetMouse()->MapCommandToButton(MouseButton::left, std::make_unique<ClickCommand>(MouseButton::left), ButtonState::Down);

	//Start Info
	const auto pStartInfoDisplay{ std::make_shared<GameObject>() };
	const auto pStartInfo{ std::make_shared<StartInfoComponent>() };
	pStartInfoDisplay->AddComponent(pStartInfo);
	pScene->Add(pStartInfoDisplay);
}

void load()
{
#if _DEBUG
	Audio::RegisterService(std::make_unique<LoggingSoundSystem>(std::make_unique<SDL_SoundSystem>()));
#else
	Audio::RegisterService(std::make_unique<SDL_SoundSystem>());
#endif
	SceneManager::GetInstance().CreateScene("ScoreScene", LoadScoreScene);
	SoundSystem& audio{ Audio::Get() };

	//Load sounds
	int bombSoundId{};
	audio.AddSound("Explosion.wav", bombSoundId, false);
	audio.Preload(bombSoundId);
	BombComponent::SetExplosionSound(bombSoundId);

	int powerUpSoundId{};
	audio.AddSound("PowerUp.wav", powerUpSoundId, false);
	PowerUp::SetSound(powerUpSoundId);

	//Music
	int menuMusicId{};
	audio.AddSound("Music/Menu.mp3", menuMusicId, true);
	audio.Play(menuMusicId, 0.4f, -1);

	int loadMusicId{};
	audio.AddSound("Music/Loading.mp3", loadMusicId, true);
	LevelLoadComponent::SetSound(loadMusicId);

	audio.AddSound("Music/Level.mp3", s_GameSoundId, true);


	//Init scores manager
	auto& manager{ BombermanManager::GetInstance() };
	manager.AddLevelPath("Bomberman_0.json");
	manager.AddLevelPath("Bomberman_1.json");
	manager.AddLevelPath("Bomberman_2.json");

	SceneManager::GetInstance().CreateScene("MenuScene", LoadMenu).Load();
}

int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}

