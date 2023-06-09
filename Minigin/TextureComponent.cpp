#include "TextureComponent.h"
#include "Renderer.h"
#include <stdexcept>
#include "Font.h"
#include "Texture2D.h"
#include <iostream>

class FontError{};

dae::TextureComponent::TextureComponent(std::shared_ptr<RenderComponent> pRenderComponent, std::shared_ptr<Font> pFont, int priority)
	:Component(priority), m_pRenderComponent{ pRenderComponent }, m_pFont{ pFont }
{
}

void dae::TextureComponent::SetTextToTexture(const std::string& text)
{
	if (m_pFont)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), m_Color);

		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto pTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);

		if (pTexture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_FreeSurface(surf);
		m_pRenderComponent->SetTexture(std::make_shared<Texture2D>(pTexture));
	}
	else
	{
		throw FontError{};
	}
}

void dae::TextureComponent::SetColorRectToTexture(int width, int height)
{
	SDL_Surface* pSurface{ SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0) };
	SDL_FillRect(pSurface, nullptr, SDL_MapRGBA(pSurface->format, m_Color.r, m_Color.g, m_Color.b, m_Color.a));

	if (pSurface == nullptr)
	{
		throw std::runtime_error(std::string("Render color rect failed: ") + SDL_GetError());
	}

	auto pTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), pSurface);

	if (pTexture == nullptr)
	{
		throw std::runtime_error(std::string("Create color rect texture from surface failed: ") + SDL_GetError());
	}

	SDL_FreeSurface(pSurface);
	m_pRenderComponent->SetTexture(std::make_shared<Texture2D>(pTexture));
}

void dae::TextureComponent::SetFont(std::shared_ptr<Font> font)
{
	m_pFont = font;
}

void dae::TextureComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
}
