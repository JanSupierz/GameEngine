#include "Keyboard.h"
#include "Command.h"

using namespace dae;

void Keyboard::Update(const SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		for (const auto& binding : m_pButtonDownCommands)
		{
			if (e.key.keysym.scancode == binding.first)
			{
				binding.second->Execute();
			}
		}
	}

	if (e.type == SDL_KEYUP)
	{
		for (const auto& binding : m_pButtonUpCommands)
		{
			if (e.key.keysym.scancode == binding.first)
			{
				binding.second->Execute();
			}
		}
	}
}

void Keyboard::UpdatePressed()
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	for (const auto& binding : m_pButtonPressedCommands)
	{
		if (state[binding.first])
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
