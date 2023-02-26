#pragma once
namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual void Update(GameObject* const pGameObject) = 0;
		virtual void Render(const GameObject* const pGameObject) = 0;

		Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
	};
}

