#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <Algorithm>
#include <iostream>
#include "SceneManager.h"
#include "Scene.h"

class ComponentTypeAlreadyAttachedException {};

dae::GameObject::GameObject(int priority)
	:m_Priority{ priority }
{
}

	void dae::GameObject::Update()
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Update();
		}

		for (auto& pChild : m_pChildren)
		{
			pChild->Update();
		}
	}

	void dae::GameObject::Render() const
	{
		for (auto& pComponent : m_pComponents)
		{
			pComponent->Render();
		}

		for (auto& pChild : m_pChildren)
		{
			pChild->Render();
		}
	}

	void dae::GameObject::SortChildren()
	{
		//Sort the children based on priority
		auto compareFunction = [&](const std::shared_ptr<GameObject>& pLhs, const std::shared_ptr<GameObject>& pRhs)
		{
			return pRhs->GetPriority() < pLhs->GetPriority();
		};

		std::sort(m_pChildren.begin(), m_pChildren.end(), compareFunction);
	}

	void dae::GameObject::AddComponent(std::shared_ptr<Component> pComponent)
	{
		//Find components of the same type
		auto typeCompare = [&](const std::shared_ptr<Component>& pComponentInVector)
		{
			return typeid(*pComponent) == typeid(*pComponentInVector);
		};

		if (std::find_if(m_pComponents.begin(), m_pComponents.end(), typeCompare) != m_pComponents.end())
		{
			throw ComponentTypeAlreadyAttachedException();
		}

		pComponent->SetOwner(this);

		m_pComponents.push_back(pComponent);
		
		//Sort the components based on priority
		auto priorityCompare = [&](const std::shared_ptr<Component>& pLhs, const std::shared_ptr<Component>& pRhs)
		{
			return *pRhs < *pLhs;
		};

		std::sort(m_pComponents.begin(), m_pComponents.end(), priorityCompare);
	}

	dae::GameObject* dae::GameObject::GetParent() const
	{
		return m_pParent;
	}

	void dae::GameObject::SetParent(GameObject* pNewParent, bool keepWorldPosition)
	{
		if (pNewParent)
		{
			if (!pNewParent->CanBeParentOf(this))
			{
				return;
			}
		}

		//Remove itself as a child from the previous parent
		if (m_pParent)
		{
			m_pParent->RemoveChild(shared_from_this());
		}
		else
		{
			SceneManager::GetInstance().GetCurrentScene()->Remove(shared_from_this());
		}

		//Set the given parent
		m_pParent = pNewParent;
		
		//Update position
		if (m_pParent == nullptr)
		{
			//Local position is now world position
			const auto position{ m_Transform.GetWorldPosition() };
			SetPosition(position.x, position.y);
		}
		else
		{
			//Add itself as a child to the given parent
			m_pParent->m_pChildren.push_back(shared_from_this());
			m_pParent->SortChildren();

			if (keepWorldPosition)
			{
				const auto position{ m_Transform.GetLocalPosition() - m_pParent->GetWorldPosition() };
				SetPosition(position.x, position.y);
			}
			else
			{
				//Recalculate world position
				SetTransformDirty();
			}
		}
	}


	void dae::GameObject::AddChild(std::shared_ptr<GameObject> pNewChild, bool keepWorldPosition)
	{
		if (!this->CanBeParentOf(pNewChild.get())) return;

		//Remove from the old parent
		GameObject* pCurrentParent{ pNewChild->m_pParent };

		if (pCurrentParent)
		{
			auto& pOtherChildren = pCurrentParent->m_pChildren;
			pOtherChildren.erase(std::remove(pOtherChildren.begin(), pOtherChildren.end(), pNewChild));
		}
		else 
		{
			SceneManager::GetInstance().GetCurrentScene()->Remove(pNewChild);
		}

		//Set itself as parent of the child
		pNewChild->m_pParent = this;

		//Add the child to its children list
		m_pChildren.push_back(pNewChild);
		SortChildren();

		//Update position
		if (keepWorldPosition)
		{
			const auto position{ pNewChild->GetLocalPosition() - m_Transform.GetWorldPosition() };
			SetPosition(position.x, position.y);
		}
		else
		{
			//Recalculate world position
			SetTransformDirty();
		}
	}

	void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> pGameObject)
	{
		//Remove the given child from the children list
		m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pGameObject));

		//Remove itself as a parent of the child.
		pGameObject->m_pParent = nullptr;

		//Add to the scene
		SceneManager::GetInstance().GetCurrentScene()->Add(pGameObject);

		//Update position, rotation and scale
		const auto position{ m_Transform.GetWorldPosition() };
		SetPosition(position.x, position.y);
	}

	bool dae::GameObject::CanBeParentOf(GameObject* pChild) const
	{
		//Object will be destroyed
		if (this->IsDestroyed())
		{
			return false;
		}

		//Grandparent is not nullptr
		if (m_pParent)
		{
			//Child is the grandparent
			if (m_pParent == pChild)
			{
				return false;
			}
			else
			{
				//Check grand-grandparents
				return m_pParent->CanBeParentOf(pChild);
			}
		}
		else
		{
			return true;
		}
	}

	void dae::GameObject::Destroy()
	{
		m_IsDestroyed = true;

		//Destroy all children
		for (auto& pChild : m_pChildren)
		{
			pChild->Destroy();
		}

		//Remove yourself from the parent
		if (m_pParent)
		{
			m_pParent->RemoveChild(shared_from_this());
		}
	}

	bool dae::GameObject::IsDestroyed() const
	{
		return m_IsDestroyed;
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

		for (const auto& pChild : m_pChildren)
		{
			pChild->SetTransformDirty();
		}
	}

	void dae::GameObject::SetPosition(float x, float y)
	{
		SetTransformDirty();
		m_Transform.SetPosition(x, y, 0.f);
	}