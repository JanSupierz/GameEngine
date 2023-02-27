#pragma once
namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		Component(int priority = 0);
		virtual ~Component() = default;
		Component(const Component& other) = default;
		Component(Component&& other) = default;
		Component& operator=(const Component& other) = default;
		Component& operator=(Component&& other) = default;

		bool operator<(const Component& other) const;

		void SetPriority(int priority);
		int GetPriority() const;

		void AddToGameObject(GameObject* pGameObject);
		void RemoveFromGameObject(GameObject* pGameObject);
	
	protected:
		GameObject* m_pGameObject{ nullptr };

	private:
		int m_Priority{ 0 };
	};
}

