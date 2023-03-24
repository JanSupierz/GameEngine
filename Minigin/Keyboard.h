#pragma once
#include <SDL.h>
#include <unordered_map>
#include <memory>
#include "Command.h"

namespace dae
{
	enum class ButtonState;

	class Keyboard
	{
	public:
		void Update(const SDL_Event& e);
		void UpdatePressed();

		void MapCommandToButton(SDL_Scancode button, std::unique_ptr<Command>&& pCommand, ButtonState state);

	private:

		std::unordered_map<SDL_Scancode, std::unique_ptr<Command>, std::hash<SDL_Scancode>> m_pButtonUpCommands;
		std::unordered_map<SDL_Scancode, std::unique_ptr<Command>, std::hash<SDL_Scancode>> m_pButtonDownCommands;
		std::unordered_map<SDL_Scancode, std::unique_ptr<Command>, std::hash<SDL_Scancode>> m_pButtonPressedCommands;
	};
}

