#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_DeltaTime = deltaTime;

	m_pCurrentScene->Update();
}

void dae::SceneManager::Render() const
{
	m_pCurrentScene->Render();
}

void dae::SceneManager::CleanUp()
{
	for (auto& pScene : m_pScenes)
	{
		if (pScene->NeedsCleanUp())
		{
			pScene->CleanUp();
		}
	}
}

float dae::SceneManager::GetDeltaTime() const
{
	return m_DeltaTime;
}

dae::Scene* dae::SceneManager::GetCurrentScene() const
{
	return m_pCurrentScene;
}

dae::Scene* dae::SceneManager::GetScene(const std::string& name) const
{
	auto pScene{ std::find_if(m_pScenes.begin(), m_pScenes.end(), [&](std::shared_ptr<Scene> pScene) { return pScene->GetName() == name; }) };

	assert(pScene != m_pScenes.end());

	return (*pScene).get();
}

void dae::SceneManager::SetCurrentScene(const std::string& name)
{
	auto pScene{ std::find_if(m_pScenes.begin(), m_pScenes.end(), [&](std::shared_ptr<Scene> pScene) { return pScene->GetName() == name; }) };

	if (pScene != m_pScenes.end())
	{
		m_pCurrentScene = (*pScene).get();
	}
}

void dae::SceneManager::ForceRemoveAllObjects()
{
	for (auto& pScene : m_pScenes)
	{
		pScene->ForceRemoveAll();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name, const std::function<void()>& loadFunction)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name, loadFunction));
	m_pScenes.push_back(scene);

	m_pCurrentScene = scene.get();

	return *scene;
}
