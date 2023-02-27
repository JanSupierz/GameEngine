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
		void RemoveComponent(std::shared_ptr<Component> pComponent);
		
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

		Transform GetTransform() const;
		glm::vec3 GetPosition() const;

		int GetPriority() const;
	private:
		Transform m_transform{};
		std::vector<std::shared_ptr<Component>> m_pComponents{};

		const int m_Priority{ 0 };
	};

}
