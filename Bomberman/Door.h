#pragma once
#include "Component.h"

namespace dae
{
	class CollisionEvent;

	class Door final : public Component
	{
	public:
		Door();
		virtual ~Door();
		Door(const Door& other) = default;
		Door(Door&& other) = default;
		Door& operator=(const Door& other) = default;
		Door& operator=(Door&& other) = default;
	};
}

