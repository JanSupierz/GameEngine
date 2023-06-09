#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class Scene;
	class GameObject;

	class CameraComponent final : public Component
	{
	public:
		CameraComponent(float ignoreDistance, int priority = 0);
		virtual ~CameraComponent();
		CameraComponent(const CameraComponent& other) = default;
		CameraComponent(CameraComponent&& other) = default;
		CameraComponent& operator=(const CameraComponent& other) = default;
		CameraComponent& operator=(CameraComponent&& other) = default;

		virtual void Update() override;

		void SetIgnoreDistance(float distance);
		void SetBounds(const glm::vec2& min, const glm::vec2& max);

		void AddObjectToFollow(GameObject* pObject);
		void RemoveObjectToFollow(GameObject* pObject);

	private:
		std::vector<GameObject*> m_pObjectsToFollow{};
		float m_IgnoreDistance;

		glm::vec2 m_BoundsMin{}, m_BoundsMax{};
	};
}