#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"

#include "UpdatePositionCommand.h"
#include <memory>
#include "RenderComponent.h"
#include "GameObject.h"
#include "Controller.h"
#include "Keyboard.h"

namespace Exercises
{
	void Drawing(dae::Scene& scene);
	void Rotating(dae::Scene& scene);
	void TrashTheCache(dae::Scene& scene);
}

	void load()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
		auto& input = dae::InputManager::GetInstance();

		Exercises::Drawing(scene);

		//Exercises::Rotating(scene);
		//Exercises::TrashTheCache(scene);

		auto pPlayer1 = std::make_shared<dae::GameObject>();

		auto pPlayer1Renderer = std::make_shared<dae::RenderComponent>();
		pPlayer1Renderer->SetTexture("Balloom.png");
		pPlayer1->AddComponent(pPlayer1Renderer);
		pPlayer1->SetPosition(100, 100);
		scene.Add(pPlayer1);
		
		//Controller
		auto controller{ input.AddController() };

		//Button down
		glm::vec3 direction{ 0.f,-100.f,0.f };
		std::unique_ptr<dae::Command> pMoveCommand{ std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction)};
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonY, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { 0.f,100.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonA, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { -100.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonX, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { 100.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonB, std::move(pMoveCommand), dae::ButtonState::Down);

		//Button pressed
		direction = { 0.f,-1.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadUp, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { 0.f,1.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadDown, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { -1.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadLeft, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { 1.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadRight, std::move(pMoveCommand), dae::ButtonState::Pressed);

		//Keyboard
		auto keyboard{ input.GetKeyboard() };

		//Key down
		direction = { 0.f,-100.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_UP, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { 0.f,100.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_DOWN, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { -100.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_LEFT, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { 100.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_RIGHT, std::move(pMoveCommand), dae::ButtonState::Down);

		//Key pressed
		direction = { 0.f,-1.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_W, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { 0.f,1.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_S, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { -1.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_A, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { 1.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_D, std::move(pMoveCommand), dae::ButtonState::Pressed);
	}

	int main(int, char* []) {
		dae::Minigin engine("../Data/");
		engine.Run(load);
		return 0;
	}