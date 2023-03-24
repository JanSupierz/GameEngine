#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Controller.h"
#include "Keyboard.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		bool ProcessInput();
		Controller* AddController();
		Keyboard* GetKeyboard();

	private:
		std::vector<std::unique_ptr<Controller>> m_pControllers{};
		Keyboard m_Keyboard;
	};

}
