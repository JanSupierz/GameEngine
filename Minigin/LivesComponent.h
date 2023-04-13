#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class TextComponent;

	class LivesComponent final : public Component, public Observer
	{
	public:
		LivesComponent(int nrLives, std::shared_ptr<TextComponent> pTextComponent = nullptr, int priority = 0);
		virtual ~LivesComponent() = default;
		LivesComponent(const LivesComponent& other) = default;
		LivesComponent(LivesComponent&& other) = default;
		LivesComponent& operator=(const LivesComponent& other) = default;
		LivesComponent& operator=(LivesComponent&& other) = default;

		virtual void OnNotify(const void* pData, const std::string& event) override;

	private:
		void TextToTexture(const std::string& name);

		std::shared_ptr<TextComponent> m_pTextComponent;
		int m_NrLives{};
	};
}

