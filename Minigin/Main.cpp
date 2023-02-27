#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

#include "Component.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "TextComponent.h"

#include "GameObject.h"

#include <iostream>

	void load()
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

		//Foreground -- Logo
		auto pLogoImage = std::make_shared<dae::GameObject>();

		auto pLogoRender = std::make_shared<dae::RenderComponent>();
		pLogoRender->SetTexture("logo.tga");

		pLogoImage->AddComponent(pLogoRender);
		pLogoImage->SetPosition(210, 200);
		scene.Add(pLogoImage);

		//Foreground -- Text
		auto pTextImage = std::make_shared<dae::GameObject>();

		auto pTextRenderer = std::make_shared<dae::RenderComponent>();
		auto pProgTextComponent = std::make_shared<dae::TextComponent>(pTextRenderer, dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
		pProgTextComponent->SetTextToTexture("Programming 4 Assignment");

		pTextImage->AddComponent(pTextRenderer);
		pTextImage->SetPosition(80, 50);
		scene.Add(pTextImage);

		//Background
		auto pBackgroundImage = std::make_shared<dae::GameObject>(100);

		auto pBackgroundRender = std::make_shared<dae::RenderComponent>(0);
		pBackgroundRender->SetTexture("background.tga");

		pBackgroundImage->AddComponent(pBackgroundRender);
		scene.Add(pBackgroundImage);

		//Make fps-counter
		auto pCounterFPS = std::make_shared<dae::GameObject>();

		//Text rendendering
		auto pFrameRateRenderer = std::make_shared<dae::RenderComponent>();
		auto pTextComponent = std::make_shared<dae::TextComponent>(pFrameRateRenderer, dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20));
		pTextComponent->SetColor({ 200,200,0 });

		//FPS component
		pCounterFPS->AddComponent(pTextComponent);

		auto pFPSComponent = std::make_shared<dae::FPSComponent>(pCounterFPS->GetComponent<dae::TextComponent>());
		//auto pFPSComponent = std::make_shared<dae::FPSComponent>(pTextComponent); //-- The same result as above

		pCounterFPS->AddComponent(pFrameRateRenderer);
		pCounterFPS->AddComponent(pFPSComponent);

		//Add to scene
		pCounterFPS->SetPosition(10.f, 10.f);
		scene.Add(pCounterFPS);

		//Test HasComponentType
		std::cout << pCounterFPS->HasComponentType<dae::RenderComponent>() << '\n';
		std::cout << pCounterFPS->HasComponentType<dae::FPSComponent>() << '\n';

		std::cout << pBackgroundImage->HasComponentType<dae::RenderComponent>() << '\n';
		std::cout << pBackgroundImage->HasComponentType<dae::FPSComponent>() << '\n';
	}

	int main(int, char* []) {
		dae::Minigin engine("../Data/");
		engine.Run(load);
		return 0;
	}