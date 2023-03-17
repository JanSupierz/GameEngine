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

		virtual ~Component() = default;
		Component(const Component& other) = default;
		Component(Component&& other) = default;
		Component& operator=(const Component& other) = default;
		Component& operator=(Component&& other) = default;

		//Owner
		GameObject* GetOwner() const;
		void SetOwner(GameObject* pGameObject);

		//Priority
		bool operator<(const Component& other) const;

		void SetPriority(int priority);
		int GetPriority() const;
	
	protected:
		explicit Component(int priority = 0);

	private:
		GameObject* m_pOwner{ nullptr };
		int m_Priority{ 0 };
	};
}

