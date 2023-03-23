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

		auto controller = input.AddController();

		glm::vec3 direction{ 0.f,-1.f,0.f };
		std::unique_ptr<dae::Command> pMoveCommand{ std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction)};
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadUp, std::move(pMoveCommand), dae::Controller::ButtonState::Pressed);

		direction = { 0.f,1.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadDown, std::move(pMoveCommand), dae::Controller::ButtonState::Pressed);

		direction = { -1.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadLeft, std::move(pMoveCommand), dae::Controller::ButtonState::Pressed);

		direction = { 1.f,0.f,0.f };
		pMoveCommand = { std::make_unique<dae::UpdatePositionCommand>(pPlayer1.get(), direction) };
		controller->MapCommandToButton(dae::Controller::ControllerButtons::DPadRight, std::move(pMoveCommand), dae::Controller::ButtonState::Pressed);
	}

	int main(int, char* []) {
		dae::Minigin engine("../Data/");
		engine.Run(load);
		return 0;
	}