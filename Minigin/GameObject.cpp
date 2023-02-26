#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <iostream>

	dae::GameObject::~GameObject() = default;

	void dae::GameObject::Update()
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Update(this);
		}
	}

	void dae::GameObject::Render() const
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Render(this);
		}
	}

	void dae::GameObject::AddComponent(Component* pComponent)
	{
		m_pComponents.push_back(pComponent);
	}

	dae::Transform dae::GameObject::GetTransform() const
	{
		return m_transform;
	}

	glm::vec3 dae::GameObject::GetPosition() const
	{
		return m_transform.GetPosition();
	}

	void dae::GameObject::SetPosition(float x, float y)
	{
		m_transform.SetPosition(x, y, 0.f);
	}