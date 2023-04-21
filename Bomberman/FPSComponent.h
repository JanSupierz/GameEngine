#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae
{
	class GameObject;

	class FPSComponent final :public Component
	{
	public:
		virtual void Update() override;

		FPSComponent(std::shared_ptr<TextComponent> pTextComponent = nullptr, float refreshTime = 0.5f, int priority = 0);
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = default;
		FPSComponent(FPSComponent&& other) = default;
		FPSComponent& operator=(const FPSComponent& other) = default;
		FPSComponent& operator=(FPSComponent&& other) = default;

	private:
		std::shared_ptr<TextComponent> m_pTextComponent;
		float m_RefreshTime;
		float m_ElapsedTime{ 0.f };
		int m_NrFrames{};
		int m_FrameRate{};
	};
}
