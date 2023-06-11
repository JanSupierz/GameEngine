#include "NextSceneCommand.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BombermanManager.h"

dae::NextSceneCommand::NextSceneCommand()
{
}

void dae::NextSceneCommand::Execute()
{
	auto pCurrent{ SceneManager::GetInstance().GetCurrentScene() };

	if (pCurrent->GetName() == "GameScene")
	{
		BombermanManager::GetInstance().NextLevel();
	}
}
