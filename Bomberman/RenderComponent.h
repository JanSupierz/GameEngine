#pragma once
#include "Component.h"
#include <string>
#include <memory>

namespace dae
{
	class GameObject;
	class Texture2D;
	class Font;

	class RenderComponent final: public Component
	{
	public:
		virtual void Render() override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> pTexture);

		RenderComponent(int priority = -2);
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = default;
		RenderComponent(RenderComponent&& other) = default;
		RenderComponent& operator=(const RenderComponent& other) = default;
		RenderComponent& operator=(RenderComponent&& other) = default;

	private:
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}

