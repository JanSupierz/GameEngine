#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;

	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::FixedUpdate(float fixedTimeStep)
{
	m_FixedTimeStep = fixedTimeStep;

	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::Render(float framePercentage)
{
	m_FramePercentage = framePercentage;

	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

float dae::SceneManager::GetDeltaTime() const
{
	return m_DeltaTime;
}

float dae::SceneManager::GetFixedTimeStep() const
{
	return m_FixedTimeStep;
}

float dae::SceneManager::GetFramePercentage() const
{
	return m_FramePercentage;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
