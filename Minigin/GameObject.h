#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace dae
{
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Update();
		void Render() const;

		GameObject(int priority = 0);
		virtual ~GameObject() = default;
		GameObject(const GameObject& other) = default;
		GameObject(GameObject&& other) = default;
		GameObject& operator=(const GameObject& other) = default;
		GameObject& operator=(GameObject&& other) = default;

		//Adding child/parent
		void SetParent(GameObject* pGameObject, bool keepWorldPosition = true);
		void AddChild(std::shared_ptr<GameObject> pGameObject, bool keepWorldPosition = true);
		void RemoveChild(std::shared_ptr<GameObject> pGameObject);

		GameObject* GetParent() const;
		bool CanBeParentOf(GameObject* pChild) const;

		//Adding components
		void AddComponent(std::shared_ptr<Component> pComponent);

		template<typename ComponentType>
		std::shared_ptr<ComponentType> GetComponent()
		{
			for (const auto& pComponent : m_pComponents)
			{
				if (auto pTargetComponent = std::dynamic_pointer_cast<ComponentType>(pComponent)) 
				{
					return pTargetComponent;
				}
			}
			return nullptr;
		}

		template<typename ComponentType>
		bool HasComponentType()
		{
			for (const auto& pComponent : m_pComponents)
			{
				if (auto pTargetComponent = std::dynamic_pointer_cast<ComponentType>(pComponent))
				{
					return true;
				}
			}
			return false;
		}

		//Transform
		void SetPosition(float x, float y);
		glm::vec3 GetWorldPosition();
		glm::vec3 GetLocalPosition() const;

		//Priority
		int GetPriority() const;

		//Destruction
		void Destroy();
		bool IsDestroyed() const;

	private:
		//Child/parent
		std::vector<std::shared_ptr<GameObject>> m_pChildren{};
		GameObject* m_pParent{};
		void SortChildren();

		//Components
		std::vector<std::shared_ptr<Component>> m_pComponents{};
		
		//Transform
		void SetTransformDirty();

		Transform m_Transform{};
		bool m_IsTransformDirty{ true };

		//Priority
		const int m_Priority{ 0 };

		//Destruction
		bool m_IsDestroyed{ false };
	};

}
