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
		void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = default;
		GameObject(GameObject&& other) = default;
		GameObject& operator=(const GameObject& other) = default;
		GameObject& operator=(GameObject&& other) = default;

		void AddComponent(Component* pComponent);

		Transform GetTransform() const;
		glm::vec3 GetPosition() const;

	private:
		Transform m_transform{};
		std::vector<Component*> m_pComponents{};

		//const int m_PriorityLevel{ 0 };
		//bool ShouldBeRemoved{ false };
	};
}
