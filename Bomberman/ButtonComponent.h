#pragma once
#include "Component.h"
#include "EventListener.h"
#include <functional>
namespace dae
{
	class MouseClickEvent;

	class ButtonComponent final : public Component, public EventListener<MouseClickEvent>
	{
	public:
		ButtonComponent(const std::function<void()>& callBack, int width, int height);
		virtual ~ButtonComponent();
		ButtonComponent(const ButtonComponent& other) = default;
		ButtonComponent(ButtonComponent&& other) = default;
		ButtonComponent& operator=(const ButtonComponent& other) = default;
		ButtonComponent& operator=(ButtonComponent&& other) = default;

		virtual void OnEvent(const MouseClickEvent& event) override;

	private:
		std::function<void()> m_CallBack;
		int m_Width, m_Height;
	};
}

