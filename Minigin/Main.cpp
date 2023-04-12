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
#include "RotatorComponent.h"
#include "GameObject.h"
#include "Controller.h"
#include "Keyboard.h"

#include "PlayerComponent.h"
#include "LivesComponent.h"
#include "Achievements.h"
#include "TextComponent.h"
#include "ResourceManager.h"

using namespace dae;

namespace Exercises
{
	void Drawing(dae::Scene& scene);
	void Rotating(dae::Scene& scene);
	void TrashTheCache(dae::Scene& scene);
}

	void load()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
		//auto& input = dae::InputManager::GetInstance();
		//auto& achievements = dae::Achievements::GetInstance();

		Exercises::Drawing(scene);

		//Exercises::Rotating(scene);
		//Exercises::TrashTheCache(scene);

		//Player 1
		const auto pPlayerObject1{ std::make_shared<GameObject>(-10) };

		const auto pPlayer1{ std::make_shared<PlayerComponent>("Player 1") };
		pPlayerObject1->AddComponent(pPlayer1);
		scene.Add(pPlayerObject1);

		//Health Display 1
		const auto pHealthDisplay1{ std::make_shared<GameObject>() };
		pHealthDisplay1->SetPosition(100.f, 100.f);
		const auto pHealthRenderer1{ std::make_shared<RenderComponent>() };
		pHealthDisplay1->AddComponent(pHealthRenderer1);

		const auto pHealthText1{ std::make_shared<TextComponent>(pHealthRenderer1) };
		pHealthText1->SetFont(dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
		pHealthDisplay1->AddComponent(pHealthText1);

		const auto pLivesObserver1{ std::make_shared<LivesComponent>(10,pHealthText1) };
		pHealthDisplay1->AddComponent(pLivesObserver1);

		pPlayer1->AddObserver(pLivesObserver1.get());
		pLivesObserver1->OnNotify(pPlayer1.get(), "PlayerDied");
		scene.Add(pHealthDisplay1);
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