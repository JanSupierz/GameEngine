#include "Scene.h"

#include "ResourceManager.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"

#include "RotatorComponent.h"

#include "GraphComponent.h"
#include "CombinedGraphComponent.h"

#include "GameObject.h"

#include <chrono>
#include <iostream>
#include <algorithm>
#include <numeric>

namespace Exercises
{
	float CalculateAverage(std::vector<float>& testResults)
	{
		const int tenPercent{ static_cast<int>(testResults.size() * 0.1f) };

		std::sort(testResults.begin(), testResults.end());

		const float totalTime{ std::accumulate(testResults.begin() + tenPercent, testResults.end() - tenPercent, 0.f) };
		const float averageTime{ totalTime / (testResults.size() - 2 * tenPercent) };

		return averageTime;
	}

	struct TransformX
	{
		float matrix[16] =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D
	{
	public:
		TransformX transform{};
		int ID{};
	};

	class GameObject3DAlt
	{
	public:
		TransformX* transform{};
		int ID{};
	};

	void TrashTheCache(dae::Scene& scene)
	{
		//Trash the cashe
		auto pTrashTheCacheObject_01{ std::make_shared<dae::GameObject>() };

		auto testFunction_01 = [&](std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)
		{
			xValues.clear();
			yValues.clear();

			int* values{ new int[nrTestValues] {} };
			std::vector<float> testResults{};

			for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
			{
				for (int testIndex{ 0 }; testIndex < nrTests; ++testIndex)
				{
					const auto start{ std::chrono::high_resolution_clock::now() };
					for (int index{}; index < nrTestValues; index += stepsize)
					{
						values[index] *= 2;
					}
					const auto end{ std::chrono::high_resolution_clock::now() };
					const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };
					testResults.push_back(elapsedTime / 1000.f);
				}

				xValues.push_back(static_cast<float>(stepsize));
				yValues.push_back(CalculateAverage(testResults));

				testResults.clear();
			}
			delete[] values;
		};

		auto pGraphComponent_01{ std::make_shared<dae::GraphComponent>(testFunction_01, "Excercise 01") };
		pTrashTheCacheObject_01->AddComponent(pGraphComponent_01);

		scene.Add(pTrashTheCacheObject_01);


		auto pTrashTheCacheObject_02{ std::make_shared<dae::GameObject>() };

		auto testFunction_02 = [&](std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)
		{
			xValues.clear();
			yValues.clear();

			GameObject3D* values{ new GameObject3D[nrTestValues] {} };
			std::vector<float> testResults{};

			for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
			{
				for (int testIndex{ 0 }; testIndex < nrTests; ++testIndex)
				{
					const auto start{ std::chrono::high_resolution_clock::now() };
					for (int index{}; index < nrTestValues; index += stepsize)
					{
						values[index].ID *= 2;
					}
					const auto end{ std::chrono::high_resolution_clock::now() };
					const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };
					testResults.push_back(elapsedTime / 1000.f);
				}

				xValues.push_back(static_cast<float>(stepsize));
				yValues.push_back(CalculateAverage(testResults));

				testResults.clear();
			}
			delete[] values;
		};

		auto testFunction_03 = [&](std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)
		{
			xValues.clear();
			yValues.clear();

			GameObject3DAlt* values{ new GameObject3DAlt[nrTestValues] {} };
			std::vector<float> testResults{};

			for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
			{
				for (int testIndex{ 0 }; testIndex < nrTests; ++testIndex)
				{
					const auto start{ std::chrono::high_resolution_clock::now() };
					for (int index{}; index < nrTestValues; index += stepsize)
					{
						values[index].ID *= 2;
					}
					const auto end{ std::chrono::high_resolution_clock::now() };
					const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };
					testResults.push_back(elapsedTime / 1000.f);
				}

				xValues.push_back(static_cast<float>(stepsize));
				yValues.push_back(CalculateAverage(testResults));

				testResults.clear();
			}
			delete[] values;
		};

		auto pGraphComponent_02{ std::make_shared<dae::CombinedGraphComponent>(testFunction_02,testFunction_03, "Excercise 02") };
		pTrashTheCacheObject_02->AddComponent(pGraphComponent_02);

		scene.Add(pTrashTheCacheObject_02);
	}

	void Rotating(dae::Scene& scene)
	{
		//Rotating balls
		auto pRotatingBallParent = std::make_shared<dae::GameObject>();

		auto pParentRenderer = std::make_shared<dae::RenderComponent>();
		pParentRenderer->SetTexture("Balloom.png");
		pRotatingBallParent->AddComponent(pParentRenderer);
		pRotatingBallParent->SetPosition(100, 100);

		const glm::vec3 origin{ 300.f,320.f,0.f };
		constexpr float length{ 50.f }, cycleTime{ 5.f };

		auto parentRotator = std::make_shared<dae::RotatorComponent>(origin, length, cycleTime);
		pRotatingBallParent->AddComponent(parentRotator);

		auto pRotatingBallChild = std::make_shared<dae::GameObject>();

		auto pChildRenderer = std::make_shared<dae::RenderComponent>();
		pChildRenderer->SetTexture("Oneal.png");
		pRotatingBallChild->AddComponent(pChildRenderer);

		auto childRotator = std::make_shared<dae::RotatorComponent>(glm::vec3{ 0.f,0.f,0.f }, length * 2.f, cycleTime);
		pRotatingBallChild->AddComponent(childRotator);

		pRotatingBallParent->AddChild(pRotatingBallChild);
		scene.Add(pRotatingBallParent);
	}

	void Drawing(dae::Scene& scene)
	{
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

		auto pFPSComponent = std::make_shared<dae::FPSComponent>(pCounterFPS->GetComponent<dae::TextComponent>(), 0.5f);
		//auto pFPSComponent = std::make_shared<dae::FPSComponent>(pTextComponent, 0.5f); //-- The same result as above

		pCounterFPS->AddComponent(pFrameRateRenderer);
		pCounterFPS->AddComponent(pFPSComponent);

		//Add to scene
		pCounterFPS->SetPosition(10.f, 10.f);
		scene.Add(pCounterFPS);
	}
}