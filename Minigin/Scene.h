#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Observer;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);

		void Add(std::shared_ptr<Observer> observer);
		void Remove(std::shared_ptr<Observer> observer);

		void RemoveAll();

		void Update();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_pObjects{};

		//Observers are not updated/rendered
		std::vector < std::shared_ptr<Observer>> m_pObservers{};

		static unsigned int m_idCounter; 
	};

}
