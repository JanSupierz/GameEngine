#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

#include "GameObject.h"

void dae::RenderComponent::Render()
{
	if (m_pTexture)
	{
		const auto& pos{ GetOwner()->GetWorldPosition() };
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
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
