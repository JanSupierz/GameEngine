#include "NextSceneCommand.h"
#include "SceneManager.h"
#include "Scene.h"

dae::NextSceneCommand::NextSceneCommand()
{
}

void dae::NextSceneCommand::Execute()
{
	auto pCurrent{ SceneManager::GetInstance().GetCurrentScene() };

	if (pCurrent->GetName() == "MenuScene")
	{
		SceneManager::GetInstance().SetCurrentScene("Demo");
	}
}
