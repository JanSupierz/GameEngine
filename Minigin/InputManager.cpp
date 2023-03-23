#include <SDL.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#include "Controller.h"

using namespace dae;

bool InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}

		//process event for ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	//Update controllers
	for (auto& pController : m_pControllers)
	{
		pController->Update();
	}

	return true;
}

Controller* dae::InputManager::AddController()
{
	const int index{ static_cast<int>(m_pControllers.size()) };
	m_pControllers.push_back(std::make_unique<Controller>(index));

	return m_pControllers.back().get();
}
