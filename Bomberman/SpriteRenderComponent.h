#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include <SDL.h>

namespace dae
{
	class GameObject;
	class Texture2D;
	class Font;

	class SpriteRenderComponent final : public Component
	{
	public:
		virtual void Render() override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		void SetSourceRect(int x, int y, int height, int width);
		void SetScale(float scale);

		SpriteRenderComponent(int x, int y, int width, int height, float scale, int priority = -2);
		virtual ~SpriteRenderComponent() = default;
		SpriteRenderComponent(const SpriteRenderComponent& other) = default;
		SpriteRenderComponent(SpriteRenderComponent&& other) = default;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = default;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = default;

	private:
		std::shared_ptr<Texture2D> m_pTexture{};
		SDL_Rect m_SourceRect;
		float m_Scale;
	};
}

