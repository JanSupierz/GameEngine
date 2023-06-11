#include "Scene.h"
#include "GameObject.h"
#include "Observer.h"
#include <algorithm>
#include "NavigationGrid.h"
#include <iostream>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

void dae::Scene::SetCamera(GameObject* pCamera)
{
	m_pCamera = pCamera;
}

GameObject* dae::Scene::GetCamera() const
{
	return m_pCamera;
}

NavigationGrid* dae::Scene::GetGrid() const
{
	return m_pGrid.get();
}

std::string dae::Scene::GetName() const
{
	return m_name;
}

void dae::Scene::Load()
{
	m_ShouldLoad = true;
}

bool dae::Scene::NeedsCleanUp() const
{
	return m_NeedsCleanUp || m_ShouldLoad;
}

Scene::Scene(const std::string& name, const std::function<void()>& loadFunction)
	:m_LoadFunction(loadFunction), m_name(name), m_pGrid{ std::make_unique<NavigationGrid>() }, m_NeedsCleanUp{ false }, m_ShouldLoad{ false }
{
}

Scene::~Scene() = default;

class GameObjectAlreadyHasParentException {};

std::shared_ptr<GameObject> Scene::Add(std::shared_ptr<GameObject> pObject)
{
	m_pNewObjects.push_back(pObject);

	return pObject;
}

void Scene::ForceRemove(std::shared_ptr<GameObject> pObject)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pObject), m_pObjects.end());
}

void Scene::ForceRemoveAll()
{
	m_pNewObjects.clear();
	m_pObjects.clear();
}

void Scene::DestroyAll()
{
	for (const auto& pObject : m_pObjects)
	{
		pObject->Destroy();
	}

	for (const auto& pObject : m_pNewObjects)
	{
		pObject->Destroy();
	}

	m_NeedsCleanUp = true;
}

void Scene::Update()
{
	for(auto& pObject : m_pObjects)
	{
		if (!pObject->IsDestroyed())
		{
			pObject->Update();
		}

		//Object is just deleted
		if (pObject->IsDestroyed())
		{
			m_NeedsCleanUp = true;
		}
	}

	//New objects
	if (!m_pNewObjects.empty())
	{
		//Add new objects
		for (auto& pObject : m_pNewObjects)
		{
			if (pObject->GetParent())
			{
				throw GameObjectAlreadyHasParentException{};
			}

			if (std::find(m_pObjects.begin(), m_pObjects.end(), pObject) != m_pObjects.end())
			{
				throw GameObjectAlreadyHasParentException{};
			}

			m_pObjects.emplace_back(std::move(pObject));
		}

		//Sort the components based on priority
		auto compareFunction = [&](const std::shared_ptr<GameObject>& pLhs, const std::shared_ptr<GameObject>& pRhs)
		{
			return pRhs->GetPriority() < pLhs->GetPriority();
		};

		std::sort(m_pObjects.begin(), m_pObjects.end(), compareFunction);
		m_pNewObjects.clear();
	}
}

void Scene::Render() const
{
	for (const auto& pObject : m_pObjects)
	{
		pObject->Render();
	}
}

void dae::Scene::CleanUp()
{
	m_NeedsCleanUp = false;

	m_pObjects.erase(std::remove_if(m_pObjects.begin(), m_pObjects.end(), 
	[&](std::shared_ptr<GameObject> pGameObject) 
	{
		return pGameObject->IsDestroyed(); 
	}), m_pObjects.end());

	m_pNewObjects.erase(std::remove_if(m_pNewObjects.begin(), m_pNewObjects.end(),
		[&](std::shared_ptr<GameObject> pGameObject)
		{
			return pGameObject->IsDestroyed();
		}), m_pNewObjects.end());

	if (m_ShouldLoad)
	{
		SceneManager::GetInstance().SetCurrentScene(m_name);

		m_pNewObjects.clear();
		m_pObjects.clear();
		m_pGrid->Clear();
		
		m_LoadFunction();
		
		m_ShouldLoad = false;
	}

	//std::cout << "--- " << m_name << " ---\n";
	//std::cout << "OldObjects: " << m_pObjects.size() << '\n';
	//std::cout << "NewObjects: " << m_pNewObjects.size() << '\n' << '\n';
}
