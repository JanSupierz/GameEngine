#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "SceneManager.h"
#include "GameObject.h"

void dae::RenderComponent::Render()
{
	if (m_pTexture)
	{
		const float framePercentage{ SceneManager::GetInstance().GetFramePercentage() };

		const glm::vec3 position{ GetOwner()->GetWorldPosition() };
		const glm::vec3 currentVelocity{ position - m_LastPosition };
		const glm::vec3 extrapolatedPosition{ position + currentVelocity * framePercentage };

		m_LastPosition = position;

		Renderer::GetInstance().RenderTexture(*m_pTexture, extrapolatedPosition.x, extrapolatedPosition.y);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

dae::RenderComponent::RenderComponent(int priority)
	:Component(priority), m_pTexture{ nullptr }
{
}
