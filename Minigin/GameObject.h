#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void Render() const;

		void SetPosition(float x, float y);

		GameObject(int priority = 0);
		virtual ~GameObject();
		GameObject(const GameObject& other) = default;
		GameObject(GameObject&& other) = default;
		GameObject& operator=(const GameObject& other) = default;
		GameObject& operator=(GameObject&& other) = default;

		void AddComponent(std::shared_ptr<Component> pComponent);

		void SetParent(GameObject* pGameObject, bool keepWorldPosition = true);

		void AddChild(GameObject* pGameObject, bool keepWorldPosition = true);
		void RemoveChild(GameObject* pGameObject);


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

		glm::vec3 GetWorldPosition();
		glm::vec3 GetLocalPosition() const;

		int GetPriority() const;

		bool CanBeParentOf(GameObject* pChild) const;

		void Destroy();
		bool IsDestroyed() const;
	private:
		void SetTransformDirty();

		std::vector<GameObject*> m_pChildren{};
		std::vector<std::shared_ptr<Component>> m_pComponents{};
		GameObject* m_pParent{};

		Transform m_Transform{};
		bool m_IsTransformDirty{ true };

		bool m_IsDestroyed{ false };

		const int m_Priority{ 0 };
	};

}
