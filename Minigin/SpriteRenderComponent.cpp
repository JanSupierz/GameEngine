#include "SpriteRenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"

void dae::SpriteRenderComponent::Render()
{
	if (m_pTexture)
	{
		const glm::vec2 position{ GetOwner()->GetWorldPosition() };

		const float factor{ m_Scale * 0.5f };
		Renderer::GetInstance().RenderTexture(*m_pTexture, position.x - m_SourceRect.w * factor, position.y - m_SourceRect.h * factor, m_SourceRect, m_Scale);
	}
}

void dae::SpriteRenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::SpriteRenderComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

void dae::SpriteRenderComponent::SetSourceRect(int x, int y, int width, int height)
{
	m_SourceRect.x = x;
	m_SourceRect.y = y;
	m_SourceRect.w = width;
	m_SourceRect.h = height;
}

void dae::SpriteRenderComponent::SetScale(float scale)
{
	m_Scale = scale;
}

dae::SpriteRenderComponent::SpriteRenderComponent(int x, int y, int width, int height, float scale, int priority)
	:Component(priority), m_pTexture{ nullptr },
	m_SourceRect{ x, y, width, height },
	m_Scale{ scale }
{
}