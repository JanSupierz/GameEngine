#include "Achievements.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <iostream>

void dae::Achievements::OnNotify(const void* pEntity, const std::string& message)
{
	if (message == "IsRotating")
	{
		const GameObject* pObject = static_cast<const GameObject*>(pEntity);

		glm::vec3 position = pObject->GetLocalPosition();

		if (m_Counter == 0)
		{
			std::cout << "Start pos: " << position.x << ' ' << position.y << '\n';
		}

		if (m_Counter == 100)
		{
			std::cout << "Pos after 100 frames: " << position.x << ' ' << position.y << '\n';
		}

		++m_Counter;
	}
}
