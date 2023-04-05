#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;

	for(auto& scene : m_pScenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render() const
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}

float dae::SceneManager::GetDeltaTime() const
{
	return m_DeltaTime;
}

dae::Scene* dae::SceneManager::GetCurrentScene() const
{
	return m_CurrentScene;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_pScenes.push_back(scene);

	m_CurrentScene = scene.get();

	return *scene;
}
