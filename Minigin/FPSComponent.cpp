#include "FPSComponent.h"
#include "SceneManager.h"
#include "TextComponent.h"
#include <string>
#include <iostream>

void dae::FPSComponent::Update()
{
	const float deltaTime{ SceneManager::GetInstance()->GetDeltaTime() };

	//frameRate = frames/elapsedTime
	++m_NrFrames;
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime >= m_RefreshTime)
	{
		const int currentFrameRate{ static_cast<int>(m_NrFrames / m_ElapsedTime) };

		//Dirty flag
		if (currentFrameRate != m_FrameRate)
		{
			m_FrameRate = currentFrameRate;
			const std::string frameRateString{ std::to_string(m_FrameRate) + " FPS" };

			if (m_pTextComponent)
			{
				//Output to render component
				m_pTextComponent->SetTextToTexture(frameRateString);
			}
		}

		m_NrFrames = 0;
		m_ElapsedTime = 0.f;
	}
}

dae::FPSComponent::FPSComponent(std::shared_ptr<TextComponent> pTextComponent, float waitingTime, int priority)
	:Component(priority), m_pTextComponent{ pTextComponent }, m_RefreshTime{ std::max(0.f, waitingTime) }
{
}
