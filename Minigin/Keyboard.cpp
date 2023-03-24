#include "Keyboard.h"
#include "Command.h"

using namespace dae;

Keyboard::Keyboard(const Uint8* pState)
	:m_pState{ pState }
{
}

void Keyboard::Update(const SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		auto pBinding{ m_pButtonDownCommands.find(e.key.keysym.scancode) };
		if (pBinding != m_pButtonDownCommands.end())
		{
			pBinding->second->Execute();
		}
	}

	if (e.type == SDL_KEYUP)
	{
		auto pBinding{ m_pButtonUpCommands.find(e.key.keysym.scancode) };
		if (pBinding != m_pButtonUpCommands.end())
		{
			pBinding->second->Execute();
		}
	}
}

void Keyboard::UpdatePressed()
{
	for (const auto& binding : m_pButtonPressedCommands)
	{
		if (m_pState[binding.first])
		{
			binding.second->Execute();
		}
	}
}

void Keyboard::MapCommandToButton(SDL_Scancode button, std::unique_ptr<Command>&& pCommand, ButtonState state)
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
