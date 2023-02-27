#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <Algorithm>
#include <iostream>

dae::GameObject::GameObject(int priority)
	:m_Priority{ priority }
{
}

dae::GameObject::~GameObject() = default;

	void dae::GameObject::Update()
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Update();
		}
	}

	void dae::GameObject::FixedUpdate()
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->FixedUpdate();
		}
	}

	void dae::GameObject::Render() const
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Render();
		}
	}

	void dae::GameObject::AddComponent(std::shared_ptr<Component> pComponent)
	{
		pComponent->AddToGameObject(this);

		m_pComponents.push_back(pComponent);
		
		//Sort the components based on priority
		auto compareFunction = [&](const std::shared_ptr<Component>& lhs, const std::shared_ptr<Component>& rhs)
		{
			return *rhs < *lhs;
		};

		std::sort(m_pComponents.begin(), m_pComponents.end(), compareFunction);
	}

	void dae::GameObject::RemoveComponent(std::shared_ptr<Component> pComponent)
	{
		m_pComponents.erase
		(
			std::remove_if(m_pComponents.begin(), m_pComponents.end(),

			[pComponent](const std::shared_ptr<Component>& ptr) 
			{
				return ptr == pComponent;
			}

		), m_pComponents.end());
	}

	dae::Transform dae::GameObject::GetTransform() const
	{
		return m_transform;
	}

	glm::vec3 dae::GameObject::GetPosition() const
	{
		return m_transform.GetPosition();
	}

	int dae::GameObject::GetPriority() const
	{
		return m_Priority;
	}

	void dae::GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.f);
	}