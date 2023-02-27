#include "FPSComponent.h"
#include "SceneManager.h"
#include "TextComponent.h"
#include <string>
#include <iostream>

void dae::FPSComponent::Update()
{
	float deltaTime{ SceneManager::GetInstance().GetDeltaTime() };

	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime >= m_RefreshTime)
	{
		m_ElapsedTime = 0.f;
		const std::string frameRateString{ std::to_string(static_cast<int>(1.f / deltaTime)) + " FPS"};

		if (m_pTextComponent)
		{
			//Output to render component
			m_pTextComponent->SetTextToTexture(frameRateString);
		}
		else
		{
			//Output to console
			std::cout << frameRateString << '\n';
		}
	}
}

dae::FPSComponent::FPSComponent(std::shared_ptr<TextComponent> pTextComponent, float waitingTime, int priority)
	:Component(priority), m_pTextComponent{ pTextComponent }, m_RefreshTime{ std::max(0.f, waitingTime) }
{
}
