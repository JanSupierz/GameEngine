#pragma once
#include "Component.h"
#include <string>
#include <memory>


namespace dae
{
	class GameObject;
	class Texture2D;

	class RenderComponent final : Component
	{
	public:
		virtual void Update(GameObject* const pGameObject) override;
		virtual void Render(const GameObject* const pGameObject);

		void SetTexture(const std::string& filename);
		//void SetText(const std::string& text);

		RenderComponent() = default;
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_texture{};

		//bool m_needsUpdate;
		//std::string m_text;
		//Transform m_transform{};
		//std::shared_ptr<Font> m_font;
		//std::shared_ptr<Texture2D> m_textTexture;
	};
}

