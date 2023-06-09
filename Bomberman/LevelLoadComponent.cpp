#include "LevelLoadComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Audio.h"

int dae::LevelLoadComponent::s_StartSoundId{ -1 };
int dae::LevelLoadComponent::s_EndSoundId{ -1 };

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
		m_TimeLeft -= SceneManager::GetInstance().GetDeltaTime();

		if (m_TimeLeft < 0.f)
		{
			Audio::Get().Play(s_EndSoundId, 1.f, -1);
			GetOwner()->Destroy();
		}
	}
}

void dae::LevelLoadComponent::SetSounds(int startId, int endId)
{
	s_StartSoundId = startId;
	s_EndSoundId = endId;
}
