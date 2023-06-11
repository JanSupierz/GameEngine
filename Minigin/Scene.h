#pragma once
#include "SceneManager.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class NavigationGrid;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, const std::function<void()>& loadFunction);
	public:
		std::shared_ptr<GameObject> Add(std::shared_ptr<GameObject> pObject);
		void ForceRemove(std::shared_ptr<GameObject> pObject);

		void ForceRemoveAll();

		void DestroyAll();

		void Update();
		void Render() const;
		void CleanUp();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void SetCamera(GameObject* pCamera);
		GameObject* GetCamera() const;

		NavigationGrid* GetGrid() const;
		std::string GetName() const;

		void Load();
		bool NeedsCleanUp() const;
	private: 
		explicit Scene(const std::string& name, const std::function<void()>& loadFunction);

		std::string m_name;
		std::vector <std::shared_ptr<GameObject>> m_pObjects{};
		std::vector <std::shared_ptr<GameObject>> m_pNewObjects{};
		static unsigned int m_idCounter; 

		GameObject* m_pCamera{};
		std::unique_ptr<NavigationGrid> m_pGrid{};

		bool m_NeedsCleanUp;
		std::function<void()> m_LoadFunction;
		bool m_ShouldLoad;
	};

}
