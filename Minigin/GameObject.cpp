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
		pComponent->SetOwner(this);

		m_pComponents.push_back(pComponent);
		
		//Sort the components based on priority
		auto compareFunction = [&](const std::shared_ptr<Component>& lhs, const std::shared_ptr<Component>& rhs)
		{
			return *rhs < *lhs;
		};

		std::sort(m_pComponents.begin(), m_pComponents.end(), compareFunction);
	}

	void dae::GameObject::SetParent(GameObject* pGameObject, bool keepWorldPosition)
	{
		if (pGameObject)
		{
			if (!pGameObject->CanBeParentOf(this))
			{
				return;
			}
		}

		auto sharedThis{ std::shared_ptr<GameObject>(this) };

		//Remove itself as a child from the previous parent
		if (m_pParent)
		{
			m_pParent->RemoveChild(sharedThis);
		}

		//Set the given parent on itself
		m_pParent = pGameObject;

		//Add itself as a child to the given parent
		if (m_pParent)
		{
			m_pParent->m_pChildren.push_back(sharedThis);
		}
		
		//Update position
		if (pGameObject == nullptr)
		{
			const auto position{ m_Transform.GetWorldPosition() };
			SetPosition(position.x, position.y);
		}
		else
		{
			m_pParent = pGameObject;

			if (keepWorldPosition)
			{
				const auto position{ m_Transform.GetLocalPosition() - m_pParent->GetWorldPosition() };
				SetPosition(position.x, position.y);
			}
		}
	}


	void dae::GameObject::AddChild(std::shared_ptr<GameObject> pGameObject, bool keepWorldPosition)
	{
		if (!this->CanBeParentOf(pGameObject.get())) return;

		//Remove from the old parent
		const auto pOldParent{ pGameObject->m_pParent };

		if (pOldParent)
		{
			pOldParent->m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pGameObject));
		}

		//Set itself as parent of the child
		pGameObject->m_pParent = this;

		//Add the child to its children list
		m_pChildren.push_back(pGameObject);

		//Update position
		if (keepWorldPosition)
		{
			const auto position{ pGameObject->GetLocalPosition() - m_Transform.GetWorldPosition() };
			SetPosition(position.x, position.y);
		}
	}

	void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> pGameObject)
	{
		//Remove the given child from the children list
		m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pGameObject));

		//Remove itself as a parent of the child.
		pGameObject->m_pParent = nullptr;

		//Update position, rotation and scale
		const auto position{ m_Transform.GetWorldPosition() };
		SetPosition(position.x, position.y);
	}

	bool dae::GameObject::CanBeParentOf(GameObject* pChild)
	{
		if (m_pParent)
		{
			return CanBeParentOf(pChild);
		}
		else
		{
			return true;
		}
	}

	glm::vec3 dae::GameObject::GetWorldPosition()
	{
		if (m_IsTransformDirty)
		{
			m_IsTransformDirty = false;
			if (m_pParent)
			{
				m_Transform.UpdateWorldPosition(m_Transform.GetLocalPosition() + m_pParent->GetWorldPosition());
				return m_Transform.GetWorldPosition();
			}
			else
			{
				m_Transform.UpdateWorldPosition(m_Transform.GetLocalPosition());
				return m_Transform.GetWorldPosition();
			}
		}
		else
		{
			return m_Transform.GetWorldPosition();
		}
	}

	glm::vec3 dae::GameObject::GetLocalPosition() const
	{
		return m_Transform.GetLocalPosition();
	}

	int dae::GameObject::GetPriority() const
	{
		return m_Priority;
	}

	void dae::GameObject::SetTransformDirty()
	{
		m_IsTransformDirty = true;

		for (auto& pChild : m_pChildren)
		{
			pChild->SetTransformDirty();
		}
	}

	void dae::GameObject::SetPosition(float x, float y)
	{
		SetTransformDirty();
		m_Transform.SetPosition(x, y, 0.f);
	}