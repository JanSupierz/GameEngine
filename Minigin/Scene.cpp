#include "Scene.h"
#include "GameObject.h"
#include "Observer.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

class GameObjectAlreadyHasParentException {};

void Scene::Add(std::shared_ptr<GameObject> pObject)
{
	m_pNewObjects.push_back(pObject);
}

void Scene::Remove(std::shared_ptr<GameObject> pObject)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pObject), m_pObjects.end());
}

void Scene::RemoveAll()
{
	m_pObjects.clear();
}

void Scene::Update()
{
	for(auto& pObject : m_pObjects)
	{
		if (!pObject->IsDestroyed())
		{
			pObject->Update();
		}
	}

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

	m_pObjects.erase(std::remove_if(m_pObjects.begin(), m_pObjects.end(), [&](std::shared_ptr<GameObject> pGameObject) {return pGameObject->IsDestroyed(); }), m_pObjects.end());
}

void Scene::Render() const
{
	for (const auto& pObject : m_pObjects)
	{
		pObject->Render();
	}
}
