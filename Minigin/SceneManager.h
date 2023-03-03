#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void FixedUpdate(float fixedTimeStep);
		void Render(float framePercentage);

		float GetDeltaTime() const;
		float GetFixedTimeStep() const;
		float GetFramePercentage() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		float m_DeltaTime{};
		float m_FixedTimeStep{};
		float m_FramePercentage{};
	};
}
