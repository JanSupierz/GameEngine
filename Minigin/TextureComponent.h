#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include <SDL_ttf.h>

namespace dae
{
	class GameObject;

	class TextureComponent final :public Component
	{
	public:
		TextureComponent(std::shared_ptr<RenderComponent> pRenderComponent = nullptr, std::shared_ptr<Font> pFont = nullptr, int priority = -1);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = default;
		TextureComponent(TextureComponent&& other) = default;
		TextureComponent& operator=(const TextureComponent& other) = default;
		TextureComponent& operator=(TextureComponent&& other) = default;

		void SetTextToTexture(const std::string& text);
		void SetColorRectToTexture(int width, int height);
		void SetFont(std::shared_ptr<Font> font);
		void SetColor(const SDL_Color& color);

	private:
		std::shared_ptr<RenderComponent> m_pRenderComponent;
		std::shared_ptr<Font> m_pFont;
		SDL_Color m_Color{ 255,255,255,255 };
	};
}
