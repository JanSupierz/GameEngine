#include "Prefabs.h"
#include "GameObject.h"
#include "InputManager.h"
#include "BombermanManager.h"

//Navigation
#include "NavigationGrid.h"
#include "NavigationNode.h"

//Rendering
#include "SpriteRenderComponent.h"
#include "TextureComponent.h"
#include "ResourceManager.h"

//Player
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "PlayerComponent.h"
#include "ColliderComponent.h"

//Enemy
#include "EnemyComponent.h"
#include "AIComponent.h"

//Cubes
#include "BlockingComponent.h"
#include "Door.h"

//Commands
#include "PlaceBombCommand.h"
#include "GridMovementCommand.h"
#include "AIWalkCommand.h"

//Camera
#include "CameraTargetComponent.h"
#include "CameraComponent.h"

#include "ButtonComponent.h"

//Black screen
#include "LevelLoadComponent.h"

using namespace dae;

std::shared_ptr<GameObject> dae::CreatePlayer(bool isEnemy, int index, const NavigationNode* const pNode, Scene& scene, 
	const int spritePosX, const int spritePosY, const float infoDisplayOffsetX, const float infoDisplayOffsetY, 
	const float infoDisplaySpacing, const int spriteCellDimensions, const float spriteCellScale)
{
	auto& manager{ BombermanManager::GetInstance() };
	bool canDetonate{ manager.CanDetonate(index) };

	//Player
	const auto pPlayerObject{ scene.Add(std::make_shared<GameObject>(-10)) };

	const auto pCollider{ std::make_shared<ColliderComponent>(glm::vec2{32.f,32.f}) };
	pPlayerObject->AddComponent(pCollider);

	const auto pPlayer{ std::make_shared<PlayerComponent>(index,canDetonate, pCollider->GetCollisionEvent()) };
	pPlayerObject->AddComponent(pPlayer);
	pPlayerObject->SetPosition(pNode->GetWorldPosition());

	const auto pPlayerRenderer{ std::make_shared<SpriteRenderComponent>(false, spritePosX,spritePosY,spriteCellDimensions,spriteCellDimensions,spriteCellScale) };
	pPlayerRenderer->SetTexture("BombermanSprites.png");
	pPlayerObject->AddComponent(pPlayerRenderer);

	//Health Display
	const auto pHealthDisplay{ scene.Add(std::make_shared<GameObject>()) };
	pHealthDisplay->SetPosition(infoDisplayOffsetX, infoDisplayOffsetY);
	const auto pHealthRenderer{ std::make_shared<RenderComponent>(true) };
	pHealthDisplay->AddComponent(pHealthRenderer);

	constexpr int fontSize{ 20 };
	const auto pHealthText{ std::make_shared<TextureComponent>(pHealthRenderer) };
	pHealthText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pHealthDisplay->AddComponent(pHealthText);

	const auto pLives{ std::make_shared<LivesComponent>(pPlayer->GetIndex(), pHealthText.get()) };
	pHealthDisplay->AddComponent(pLives);

	//Score Display 1
	const auto pScoreDisplay{ scene.Add(std::make_shared<GameObject>()) };
	pScoreDisplay->SetPosition(infoDisplayOffsetX, infoDisplayOffsetY + infoDisplaySpacing);
	const auto pScoreRenderer{ std::make_shared<RenderComponent>(true) };
	pScoreDisplay->AddComponent(pScoreRenderer);

	const auto pScoreText{ std::make_shared<TextureComponent>(pScoreRenderer) };
	pScoreText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pScoreDisplay->AddComponent(pScoreText);

	const auto pScore{ std::make_shared<ScoreComponent>(pPlayer->GetIndex(), pScoreText.get()) };
	pScoreDisplay->AddComponent(pScore);

	if (isEnemy)
	{
		const auto pEnemy{ std::make_shared<EnemyComponent>(DeathType::BalloomPlayer, pCollider->GetCollisionEvent()) };
		pPlayerObject->AddComponent(pEnemy);
	}

	//Camera Target
	auto pCameraTarget{ std::make_shared<CameraTargetComponent>() };
	pPlayerObject->AddComponent(pCameraTarget);

	pCameraTarget->SetCamera(scene.GetCamera()->GetComponent<CameraComponent>().get());

	manager.SetPlayerObject(pPlayerObject.get());

	return pPlayerObject;
}

std::shared_ptr<GameObject> dae::CreateEnemy(const NavigationNode* const pNode, DeathType enemyType, const float speed, Scene& scene,
	const int spritePosX, const int spritePosY, const int spriteCellDimensions, const float spriteCellScale)
{
	//Enemy
	const auto pEnemyObject{ scene.Add(std::make_shared<GameObject>(-10)) };

	constexpr float margin{ 4.f };
	const float dimension{ static_cast<float>(pNode->GetGrid()->GetSmallerDimension()) - margin };

	const auto pCollider{ std::make_shared<ColliderComponent>(glm::vec2{dimension, dimension}) };
	pEnemyObject->AddComponent(pCollider);

	const auto pEnemy{ std::make_shared<EnemyComponent>(enemyType, pCollider->GetCollisionEvent()) };
	pEnemyObject->AddComponent(pEnemy);
	pEnemyObject->SetPosition(pNode->GetWorldPosition());

	const auto pEnemyRenderer{ std::make_shared<SpriteRenderComponent>(false, spritePosX,spritePosY,spriteCellDimensions,spriteCellDimensions,spriteCellScale) };
	pEnemyRenderer->SetTexture("BombermanSprites.png");
	pEnemyObject->AddComponent(pEnemyRenderer);

	bool isSmart{ enemyType == DeathType::Oneal || enemyType == DeathType::Minvo };

	auto pAIWalkCommand{ std::make_unique<AIWalkCommand>(pEnemyObject.get(),speed, isSmart) };

	const auto pAI{ std::make_shared<AIComponent>(std::move(pAIWalkCommand)) };
	pEnemyObject->AddComponent(pAI);

	return pEnemyObject;
}

void dae::CreateCube(Scene& scene, const int spritePosX, const int spritePosY, const int spriteCellDimensions, const float spriteCellScale, const glm::vec2& position)
{
	const auto pGameObject{ scene.Add(std::make_shared<GameObject>()) };

	const auto pRenderer{std::make_shared<SpriteRenderComponent>(false, spritePosX, spritePosY, spriteCellDimensions, spriteCellDimensions, spriteCellScale)};

	pRenderer->SetTexture("BombermanSprites.png");

	pGameObject->AddComponent(pRenderer);
	pGameObject->SetPosition(position);
}

void dae::CreateBlockingCube(Scene& scene, NavigationNode* pNode)
{
	const auto pCubeObject{ scene.Add(std::make_shared<GameObject>(-10)) };

	const auto pCube{ std::make_shared<BlockingComponent>(pNode) };
	pCubeObject->AddComponent(pCube);
	pCubeObject->SetPosition(pNode->GetWorldPosition());

	const auto pCubeRenderer{ std::make_shared<SpriteRenderComponent>(false, 4 * 16,3 * 16,16,16,2.f) };
	pCubeRenderer->SetTexture("BombermanSprites.png");
	pCubeObject->AddComponent(pCubeRenderer);
}

void dae::CreateDoor(Scene& scene, NavigationNode* pNode)
{
	const auto pDoorObject{ scene.Add(std::make_shared<GameObject>(-7)) };

	const auto pCollider{ std::make_shared<ColliderComponent>(glm::vec2{5.f,5.f}) };
	pDoorObject->AddComponent(pCollider);

	const auto pDoor{ std::make_shared<Door>() };
	pDoorObject->AddComponent(pDoor);

	const auto pDoorRenderer{ std::make_shared<SpriteRenderComponent>(false, 11 * 16,3 * 16,16,16,2.f) };
	pDoorRenderer->SetTexture("BombermanSprites.png");
	pDoorObject->AddComponent(pDoorRenderer);

	pDoorObject->SetPosition(pNode->GetWorldPosition());
}

void dae::CreateText(Scene& scene, const glm::vec2& position, const std::string& text, int fontSize)
{
	const auto pTextObject{ scene.Add(std::make_shared<GameObject>()) };

	const auto pTextRenderer{ std::make_shared<RenderComponent>(true) };
	pTextObject->AddComponent(pTextRenderer);

	const auto pText{ std::make_shared<TextureComponent>(pTextRenderer) };
	pText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
	pText->SetTextToTexture(text);

	pTextObject->AddComponent(pText);
	pTextObject->SetPosition({ position.x, position.y });
}

void dae::CreateButton(Scene& scene, const glm::vec2& position, const std::string& text, int fontSize, const std::function<void()>& callBack)
{
	//Buttons
	const auto pButtonObject{ scene.Add(std::make_shared<GameObject>()) };
	pButtonObject->SetPosition(position);

	{
		const auto pTextObject{ std::make_shared<GameObject>() };
		pButtonObject->AddChild(pTextObject, false);

		const auto pTextRenderer{ std::make_shared<RenderComponent>(true) };
		pTextObject->AddComponent(pTextRenderer);

		const auto pText{ std::make_shared<TextureComponent>(pTextRenderer) };
		pText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
		pText->SetTextToTexture(text);

		pTextObject->AddComponent(pText);
		pTextObject->SetPosition({ 10.f, -fontSize * 0.5f + 2.f });

		const auto pButton{ std::make_shared<ButtonComponent>(callBack, 100,fontSize) };
		pTextObject->AddComponent(pButton);
	}

	const auto pRenderer{ std::make_shared<SpriteRenderComponent>(true, 11 * 16,0,16,16,0.5f,1) };
	pButtonObject->AddComponent(pRenderer);
	pRenderer->SetTexture("BombermanSprites.png");
}

void dae::CreateImage(Scene& scene, const glm::vec2& position, int width, int height, float scale)
{
	const auto pImageObject{ scene.Add(std::make_shared<GameObject>(2)) };
	pImageObject->SetPosition(position);

	const auto pRenderer{ std::make_shared<SpriteRenderComponent>(true,0,0,width,height,scale) };
	pImageObject->AddComponent(pRenderer);
	pRenderer->SetTexture("Title.png");
}

std::shared_ptr<GameObject> dae::CreateLoadingScreen(Scene& scene, int width, int height, const std::string& text, int fontSize, int priority)
{
	//Buttons 
	const auto pScreenObject{ scene.Add(std::make_shared<GameObject>(priority)) };
	pScreenObject->AddComponent(std::make_shared<LevelLoadComponent>());

	{
		const auto pTextObject{ std::make_shared<GameObject>() };
		pScreenObject->AddChild(pTextObject, false);

		const auto pTextRenderer{ std::make_shared<RenderComponent>(true) };
		pTextObject->AddComponent(pTextRenderer);

		const auto pText{ std::make_shared<TextureComponent>(pTextRenderer) };
		pText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize));
		pText->SetTextToTexture(text);

		pTextObject->AddComponent(pText);
		pTextObject->SetPosition({ width * 0.5f - 40.f, (height - fontSize) * 0.5f + 2.f });
	}

	const auto pRenderer{ std::make_shared<RenderComponent>(true) };
	pScreenObject->AddComponent(pRenderer);

	const auto pText{ std::make_shared<TextureComponent>(pRenderer) };
	pText->SetColor(SDL_Color{ 0, 0, 0, 255 });
	pText->SetColorRectToTexture(width, height);

	return pScreenObject;
}
