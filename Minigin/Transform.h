#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec2& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec2& GetWorldPosition() const { return m_WorldPosition; }

		void UpdateWorldPosition(const glm::vec2& position);

		void SetPosition(float x, float y);

	private:
		glm::vec2 m_LocalPosition;
		glm::vec2 m_WorldPosition;
	};
}
