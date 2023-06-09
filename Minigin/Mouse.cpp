#include "Mouse.h"

using namespace dae;

Mouse::Mouse()
{
}

void Mouse::Update(const SDL_Event& e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		const auto pBinding{ m_pButtonDownCommands.find(static_cast<MouseButton>(e.button.button)) };
		if (pBinding != m_pButtonDownCommands.end())
		{
			pBinding->second->SetValue({ posX,posY });
			pBinding->second->Execute();
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP)
	{
		const auto pBinding{ m_pButtonUpCommands.find(static_cast<MouseButton>(e.button.button)) };
		if (pBinding != m_pButtonUpCommands.end())
		{
			pBinding->second->SetValue({ posX,posY });
			pBinding->second->Execute();
		}
	}
}

void Mouse::UpdateClicked()
{
	Uint32 mouseState{ SDL_GetMouseState(&posX, &posY) };

	for (const auto& binding : m_pButtonPressedCommands)
	{
		if (mouseState & SDL_BUTTON(static_cast<int>(binding.first)))
		{
			binding.second->SetValue({ posX,posY });
			binding.second->Execute();
		}
	}
}

void Mouse::MapCommandToButton(MouseButton button, std::unique_ptr<Value2DCommand>&& pCommand, ButtonState state)
{
	switch (state)
	{
	case dae::ButtonState::Up:
		m_pButtonUpCommands.emplace(button, std::move(pCommand));
		break;
	case dae::ButtonState::Down:
		m_pButtonDownCommands.emplace(button, std::move(pCommand));
		break;
	case dae::ButtonState::Pressed:
		m_pButtonPressedCommands.emplace(button, std::move(pCommand));
		break;
	default:
		break;
	}
}

void dae::Mouse::ClearCommands()
{
	m_pButtonUpCommands.clear();
	m_pButtonDownCommands.clear();
	m_pButtonPressedCommands.clear();
}
