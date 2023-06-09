#pragma once
#include "RenderComponent.h"
#include <string>
#include <memory>
#include <SDL.h>

namespace dae
{
	class SpriteRenderComponent final : public RenderComponent
	{
	public:
		virtual void Render() override;

		void SetSourceRect(int x, int y, int height, int width);
		void SetScale(float scale);

		SpriteRenderComponent(bool isHUD, int x, int y, int width, int height, float scale, int priority = -2);
		virtual ~SpriteRenderComponent() = default;
		SpriteRenderComponent(const SpriteRenderComponent& other) = default;
		SpriteRenderComponent(SpriteRenderComponent&& other) = default;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = default;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = default;

	private:
		SDL_Rect m_SourceRect;
		float m_Scale;
	};
}

