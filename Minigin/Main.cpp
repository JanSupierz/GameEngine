#include <SDL.h>
#include <steam_api.h>
#include <iostream>

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
#include "UpdatePosition1DCommand.h"
#include "UpdatePosition2DCommand.h"
#include "SetPositionCommand.h"

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

		float setSpeed{ 100.f };
		float updateSpeed{ 150.f };

		auto pPlayer1 = std::make_shared<dae::GameObject>();

		auto pPlayer1Renderer = std::make_shared<dae::RenderComponent>();
		pPlayer1Renderer->SetTexture("Balloom.png");
		pPlayer1->AddComponent(pPlayer1Renderer);
		pPlayer1->SetPosition(100, 100);
		scene.Add(pPlayer1);
		
		//Controller
		auto controller{ input.AddController() };

		std::unique_ptr<dae::Value2DCommand> pThumbstickCommand{ std::make_unique<dae::UpdatePosition2DCommand>(pPlayer1.get(), updateSpeed) };
		controller->MapCommandToThumbstick(dae::Controller::ControllerThumbsticks::LeftThumbstick, std::move(pThumbstickCommand));

		glm::vec2 triggerMoveDirection{ 0.f, updateSpeed };
		std::unique_ptr<dae::Value1DCommand> pTriggerCommand{ std::make_unique<dae::UpdatePosition1DCommand>(pPlayer1.get(), triggerMoveDirection) };
		controller->MapCommandToTrigger(dae::Controller::ControllerTriggers::LeftTrigger, std::move(pTriggerCommand));

		//Button down (Set position command)
		glm::vec3 direction{ 0.f,-setSpeed,0.f };
		std::unique_ptr<dae::Command> pMoveCommand{ std::make_unique<dae::SetPositionCommand>(pPlayer1.get(), direction)};
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonY, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { 0.f,setSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonA, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { -setSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonX, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { setSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonB, std::move(pMoveCommand), dae::ButtonState::Down);

		//Button pressed (Update position command)
		direction = { 0.f,-updateSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadUp, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { 0.f,updateSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadDown, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { -updateSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadLeft, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { updateSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadRight, std::move(pMoveCommand), dae::ButtonState::Pressed);

		setSpeed *= 0.5f;
		updateSpeed *= 0.5f;

		auto pPlayer2 = std::make_shared<dae::GameObject>();

		auto pPlayer2Renderer = std::make_shared<dae::RenderComponent>();
		pPlayer2Renderer->SetTexture("Oneal.png");
		pPlayer2->AddComponent(pPlayer2Renderer);
		pPlayer2->SetPosition(200, 200);
		scene.Add(pPlayer2);

		//Keyboard
		auto keyboard{ input.GetKeyboard() };

		//Key down
		direction = { 0.f,-setSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_UP, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { 0.f,setSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_DOWN, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { -setSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_LEFT, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { setSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_RIGHT, std::move(pMoveCommand), dae::ButtonState::Down);

		//Key pressed
		direction = { 0.f,-updateSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_W, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { 0.f,updateSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_S, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { -updateSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_A, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { updateSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		keyboard->MapCommandToButton(SDL_SCANCODE_D, std::move(pMoveCommand), dae::ButtonState::Pressed);

		//Controller nr 2
		controller = { input.AddController() };

		//Button down
		direction = { 0.f,-setSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonY, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { 0.f,setSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonA, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { -setSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonX, std::move(pMoveCommand), dae::ButtonState::Down);

		direction = { setSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::SetPositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonB, std::move(pMoveCommand), dae::ButtonState::Down);

		//Button pressed
		direction = { 0.f,-updateSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadUp, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { 0.f,updateSpeed,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadDown, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { -updateSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadLeft, std::move(pMoveCommand), dae::ButtonState::Pressed);

		direction = { updateSpeed,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer2.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadRight, std::move(pMoveCommand), dae::ButtonState::Pressed);
	}

	int main(int, char*[])
	{
		if (!SteamAPI_Init())
		{
			std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
			return 1;
		}
		else
		{
			std::cout << "Successfully initialized steam." << std::endl;
		}

		dae::Minigin engine("../Data/");
		engine.Run(load);

		SteamAPI_Shutdown();
		return 0;
	}