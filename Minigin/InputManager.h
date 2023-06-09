#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Controller.h"
#include "Keyboard.h"
#include "Mouse.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		void ClearCommands();

		bool ProcessInput();
		Controller* AddController();
		Keyboard* GetKeyboard();
		Mouse* GetMouse();

	private:
		std::vector<std::unique_ptr<Controller>> m_pControllers{};
		Keyboard m_Keyboard;
		Mouse m_Mouse;
	};

}
