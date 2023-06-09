#include "SpriteRenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SpriteRenderComponent::Render()
{
	Texture2D* pTexture{ GetTexture() };

	if (pTexture)
	{
		const glm::vec2 position
		{
			(IsHUD()
				? GetOwner()->GetWorldPosition()
				: GetOwner()->GetWorldPosition() - SceneManager::GetInstance().GetCurrentScene()->GetCamera()->GetWorldPosition()
			)
		};

		const float factor{ m_Scale * 0.5f };
		Renderer::GetInstance().RenderTexture(*pTexture, position.x - m_SourceRect.w * factor, position.y - m_SourceRect.h * factor, m_SourceRect, m_Scale);
	}
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

dae::SpriteRenderComponent::SpriteRenderComponent(bool isHUD, int x, int y, int width, int height, float scale, int priority)
	:RenderComponent(isHUD, priority),
	m_SourceRect{ x, y, width, height },
	m_Scale{ scale }
{
}