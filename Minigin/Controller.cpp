#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#include "Controller.h"
#include "Command.h"

using namespace dae;

class Controller::ControllerImpl final
{
public:
	ControllerImpl(int controllerIndex)
		:m_ControllerIndex(controllerIndex)
	{
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges{ m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons };
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	}

	bool IsUpThisFrame(unsigned int button) const
	{
		return m_ButtonsReleasedThisFrame & button;
	}

	bool IsPressed(unsigned int button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	}

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	const int m_ControllerIndex;
};

Controller::Controller(int controllerIndex)
	:m_pImpl(new ControllerImpl{ controllerIndex })
{
}

dae::Controller::~Controller()
{
	delete m_pImpl;
}

void Controller::Update()
{
	m_pImpl->Update();

	for (const auto& binding : m_pButtonDownCommands)
	{
		if (IsDown(binding.first))
		{
			binding.second->Execute();
		}
	}
	
	for (const auto& binding : m_pButtonUpCommands)
	{
		if (IsUp(binding.first))
		{
			binding.second->Execute();
		}
	}
	
	for (const auto& binding : m_pButtonPressedCommands)
	{
		if (IsPressed(binding.first))
		{
			binding.second->Execute();
		}
	}
}

bool dae::Controller::IsDown(ControllerButtons button) const
{
	return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::Controller::IsUp(ControllerButtons button) const
{
	return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool dae::Controller::IsPressed(ControllerButtons button) const
{
	return m_pImpl->IsPressed(static_cast<unsigned int>(button));
}

void dae::Controller::MapCommandToButton(ControllerButtons button, std::unique_ptr<Command>&& pCommand, ButtonState state)
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

