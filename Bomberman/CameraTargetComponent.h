#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class CameraComponent;

	class CameraTargetComponent final : public Component
	{
	public:
		CameraTargetComponent(int priority = 0);
		virtual ~CameraTargetComponent();
		CameraTargetComponent(const CameraTargetComponent& other) = default;
		CameraTargetComponent(CameraTargetComponent&& other) = default;
		CameraTargetComponent& operator=(const CameraTargetComponent& other) = default;
		CameraTargetComponent& operator=(CameraTargetComponent&& other) = default;

		void SetCamera(CameraComponent* pCamera);
	private:
		CameraComponent* m_pCamera;
	};
}