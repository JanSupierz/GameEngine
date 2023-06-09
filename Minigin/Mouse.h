#pragma once
#include <unordered_map>
#include <memory>
#include "Value2DCommand.h"
#include <SDL.h>

namespace dae
{
	enum class ButtonState;

	enum class MouseButton
	{
		left = 1, middle = 2, right = 3
	};

	class Mouse final
	{
	public:
		explicit Mouse();
		virtual ~Mouse() = default;

		void Update(const SDL_Event& e);
		void UpdateClicked();

		void MapCommandToButton(MouseButton button, std::unique_ptr<Value2DCommand>&& pCommand, ButtonState state);
		void ClearCommands();
	private:
		int posX{}, posY{};

		std::unordered_map<MouseButton, std::unique_ptr<Value2DCommand>, std::hash<MouseButton>> m_pButtonUpCommands;
		std::unordered_map<MouseButton, std::unique_ptr<Value2DCommand>, std::hash<MouseButton>> m_pButtonDownCommands;
		std::unordered_map<MouseButton, std::unique_ptr<Value2DCommand>, std::hash<MouseButton>> m_pButtonPressedCommands;
	};
}

