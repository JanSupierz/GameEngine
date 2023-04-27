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

#include "UpdatePositionCommand.h"
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
#include "AIMoveComponent.h"

using namespace dae;

namespace Exercises
{
	void Drawing(Scene& scene);
	void Rotating(Scene& scene);
	void TrashTheCache(Scene& scene);
}

void load()
{
	auto& grid = NavigationGrid::GetInstance();
	grid.AddNode(0, 0);
	grid.AddNode(1, 0);
	grid.AddNode(1, 1);
	grid.AddNode(2, 1);
	grid.AddNode(3, 1);
	grid.AddNode(3, 2);
	grid.AddNode(2, 2);


	//grid.AddNode(0, 1);
	//grid.AddNode(1, 1);
	//grid.AddNode(2, 1);
	//grid.AddNode(3, 1);
	//grid.AddNode(4, 1);
	//
	//grid.AddNode(0, 2);
	//grid.AddNode(1, 2);
	//grid.AddNode(2, 2);
	//grid.AddNode(3, 2);
	//grid.AddNode(4, 2);
	//
	//grid.AddNode(0, 3);
	//grid.AddNode(1, 3);
	//grid.AddNode(2, 3);
	//grid.AddNode(3, 3);
	//grid.AddNode(4, 3);
	//
	//grid.AddNode(0, 4);
	//grid.AddNode(1, 4);
	//grid.AddNode(2, 4);
	//grid.AddNode(3, 4);
	//grid.AddNode(4, 4);

	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& input = InputManager::GetInstance();
	ScoresManager::GetInstance();

	Exercises::Drawing(scene);

	//Exercises::Rotating(scene);
	//Exercises::TrashTheCache(scene);

	constexpr int fontSize{ 20 };

	//Player 1
	const auto pPlayerObject1{ std::make_shared<GameObject>(-10) };

	const auto pPlayer1{ std::make_shared<PlayerComponent>("Player 1",100) };
	pPlayerObject1->AddComponent(pPlayer1);
	pPlayerObject1->SetPosition(200.f, 200.f);

	const auto pPlayerRenderer1{ std::make_shared<RenderComponent>() };
	pPlayerRenderer1->SetTexture("Oneal.png");
	pPlayerObject1->AddComponent(pPlayerRenderer1);

	scene.Add(pPlayerObject1);

	//Health Display 1
	const auto pHealthDisplay1{ std::make_shared<GameObject>() };
	pHealthDisplay1->SetPosition(100.f, 100.f);
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
	pScoreDisplay1->SetPosition(100.f, 150.f);
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

	pController1->MapCommandToButton(Controller::ControllerButtons::DPadLeft, std::make_unique<UpdatePositionCommand>(pPlayerObject1.get(), glm::vec2{ -speed,0.f }), ButtonState::Pressed);
	pController1->MapCommandToButton(Controller::ControllerButtons::DPadRight, std::make_unique<UpdatePositionCommand>(pPlayerObject1.get(), glm::vec2{ speed,0.f }), ButtonState::Pressed);
	pController1->MapCommandToButton(Controller::ControllerButtons::DPadUp, std::make_unique<UpdatePositionCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,-speed }), ButtonState::Pressed);
	pController1->MapCommandToButton(Controller::ControllerButtons::DPadDown, std::make_unique<UpdatePositionCommand>(pPlayerObject1.get(), glm::vec2{ 0.f,speed }), ButtonState::Pressed);

	//Player 2
	const auto pPlayerObject2{ std::make_shared<GameObject>(-10) };

	const auto pAI{ std::make_shared<AIMoveComponent>(150.f) };
	pPlayerObject2->AddComponent(pAI);
	pPlayerObject2->SetPosition(50.f, 50.f);

	const auto pPlayer2{ std::make_shared<PlayerComponent>("Player 2",100) };
	pPlayerObject2->AddComponent(pPlayer2);

	const auto pPlayerRenderer2{ std::make_shared<RenderComponent>() };
	pPlayerRenderer2->SetTexture("Balloom.png");
	pPlayerObject2->AddComponent(pPlayerRenderer2);

	scene.Add(pPlayerObject2);

	//Health Display 2
	const auto pHealthDisplay2{ std::make_shared<GameObject>() };
	pHealthDisplay2->SetPosition(300.f, 100.f);
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
	pScoreDisplay2->SetPosition(300.f, 150.f);
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

	pKeyboard1->MapCommandToButton(SDL_SCANCODE_LEFT, std::make_unique<UpdatePositionCommand>(pPlayerObject2.get(), glm::vec2{ -speed,0.f }), ButtonState::Pressed);
	pKeyboard1->MapCommandToButton(SDL_SCANCODE_RIGHT, std::make_unique<UpdatePositionCommand>(pPlayerObject2.get(), glm::vec2{ speed,0.f }), ButtonState::Pressed);
	pKeyboard1->MapCommandToButton(SDL_SCANCODE_UP, std::make_unique<UpdatePositionCommand>(pPlayerObject2.get(), glm::vec2{ 0.f,-speed }), ButtonState::Pressed);
	pKeyboard1->MapCommandToButton(SDL_SCANCODE_DOWN, std::make_unique<UpdatePositionCommand>(pPlayerObject2.get(), glm::vec2{ 0.f,speed }), ButtonState::Pressed);

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