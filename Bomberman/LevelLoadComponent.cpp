#include "LevelLoadComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Audio.h"

int dae::LevelLoadComponent::s_StartSoundId{ -1 };

dae::LevelLoadComponent::LevelLoadComponent()
	:m_TimeLeft{ 3.f }, m_IsInit{ false }
{
}

dae::LevelLoadComponent::~LevelLoadComponent()
{
}

void dae::LevelLoadComponent::Update()
{
	if (!m_IsInit)
	{
		Audio::Get().Play(s_StartSoundId, 1.f, 1);
		m_IsInit = true;
	}
	else
	{
		if (m_TimeLeft >= 0.f)
		{
			m_TimeLeft -= SceneManager::GetInstance().GetDeltaTime();

			if (m_TimeLeft < 0.f)
			{
				auto& manager{ SceneManager::GetInstance() };
				manager.GetScene("GameScene")->Load();
			}
		}
	}
}

void dae::LevelLoadComponent::SetSound(int startId)
{
	s_StartSoundId = startId;
}
