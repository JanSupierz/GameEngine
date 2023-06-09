#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"

void dae::RenderComponent::Render()
{
	if (m_pTexture)
	{
		const glm::vec2 position
		{
			(IsHUD()
				? GetOwner()->GetWorldPosition()
				: GetOwner()->GetWorldPosition() - SceneManager::GetInstance().GetCurrentScene()->GetCamera()->GetWorldPosition()
			)
		};

		Renderer::GetInstance().RenderTexture(*m_pTexture, position.x, position.y);
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

dae::RenderComponent::RenderComponent(bool isHUD, int priority)
	:Component(priority), m_pTexture{ nullptr }, m_IsHUD{ isHUD }
{
}

void dae::RenderComponent::SetHUD(bool isHUD)
{
	m_IsHUD = isHUD;
}

bool dae::RenderComponent::IsHUD() const
{
	return m_IsHUD;
}

dae::Texture2D* dae::RenderComponent::GetTexture() const
{
	return m_pTexture.get();
}
