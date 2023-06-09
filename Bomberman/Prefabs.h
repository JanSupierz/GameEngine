#pragma once
#include <memory>
#include <string>
#include "Scene.h"
#include <functional>

namespace dae
{
	class GameObject;
	class NavigationNode;
	enum class DeathType;
	
	std::shared_ptr<GameObject> CreatePlayer(const NavigationNode* const pNode,
		const std::string& name, const float playerSpeed, Scene& scene, const int spritePosX,
		const int spritePosY, const float infoDisplayOffsetX = 10.f, const float infoDisplayOffsetY = 20.f,
		const float infoDisplaySpacing = 20.f, const int spriteCellDimensions = 16, const float spriteCellScale = 2.f);

	std::shared_ptr<GameObject> CreateEnemy(const NavigationNode* const pNode, DeathType enemyType, const float speed, 
		Scene& scene, const int spritePosX, const int spritePosY, const int spriteCellDimensions = 16, const float spriteCellScale = 2.f);

	void CreateCube(Scene& scene, const int spritePosX, const int spritePosY, const int spriteCellDimensions, const float spriteCellScale, const glm::vec2& position);
	void CreateBlockingCube(Scene& scene, NavigationNode* pNode);
	void CreateDoor(Scene& scene, NavigationNode* pNode);
	void CreateButton(Scene& scene, const glm::vec2& position, const std::string& text, int fontSize, const std::function<void()>& callBack);
	void CreateImage(Scene& scene, const glm::vec2& position, int width, int height, float scale);

	std::shared_ptr<GameObject> CreateLoadingScreen(Scene& scene, int width, int height, const std::string& text, int fontSize, int priority);
}
