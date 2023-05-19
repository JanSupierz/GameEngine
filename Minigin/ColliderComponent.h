#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include <memory>
#include "CollisionEvent.h"

namespace dae
{
	class ColliderComponent final : public Component
	{
	public:
		ColliderComponent(const glm::vec2& size, int priority = -2);
		virtual ~ColliderComponent();
		ColliderComponent(const ColliderComponent& other) = default;
		ColliderComponent(ColliderComponent&& other) = default;
		ColliderComponent& operator=(const ColliderComponent& other) = default;
		ColliderComponent& operator=(ColliderComponent&& other) = default;

		virtual void Update() override;
		bool CheckCollision(const glm::vec2& otherPosition, const glm::vec2& otherSize);

		void SetSize(const glm::vec2& size) { m_Size = size; };
		void SetLayer(unsigned int layer);

		glm::vec2 GetSize() const { return m_Size; };
		unsigned int GetLayer() const { return m_Layer; };

		CollisionEvent* GetCollisionEvent() const { return m_pCollisionEvent.get(); };

	private:
		glm::vec2 m_Size;
		unsigned int m_Layer{ 0 };

		std::unique_ptr<CollisionEvent> m_pCollisionEvent{};
	};
}

