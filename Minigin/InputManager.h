#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Controller.h"

namespace dae
{
	class Controller;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		Controller* AddController();

	private:
		std::vector<std::unique_ptr<Controller>> m_pControllers{};
	};

}
