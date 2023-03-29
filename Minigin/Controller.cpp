#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#include "Controller.h"

#include "Command.h"
#include "SingleValuecommand.h"
#include "AxisValuecommand.h"

#include <glm/glm.hpp>
#include <utility>

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
	
	glm::vec2 GetValue(ControllerThumbsticks thumbstick) const
	{
		if (thumbstick == ControllerThumbsticks::LeftThumbstick)
		{
			return glm::vec2{ m_CurrentState.Gamepad.sThumbLX,m_CurrentState.Gamepad.sThumbLY };
		}
		else
		{
			return glm::vec2{ m_CurrentState.Gamepad.sThumbRX,m_CurrentState.Gamepad.sThumbRY };
		}
	}

	float GetValue(ControllerTriggers trigger) const
	{
		if (trigger == ControllerTriggers::LeftTrigger)
		{
			return m_CurrentState.Gamepad.bLeftTrigger;
		}
		else
		{
			return m_CurrentState.Gamepad.bRightTrigger;
		}
	}

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	const int m_ControllerIndex;
};

Controller::Controller(int controllerIndex, bool invertY)
	:m_pImpl(new ControllerImpl{ controllerIndex }), m_InvertY{ invertY }
{
}

dae::Controller::~Controller()
{
	delete m_pImpl;
}

void Controller::Update()
{
	m_pImpl->Update();

	//Buttons
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

	//Triggers
	for (const auto& binding : m_pTriggerCommands)
	{
		UpdateBinding(binding);
	}

	//Thumbsticks
	for (auto& binding : m_pThumbstickCommands)
	{
		UpdateBinding(binding);
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

void dae::Controller::UpdateBinding(const std::pair<const ControllerTriggers, std::unique_ptr<SingleValueCommand>>& binding) const
{
	constexpr float maxValue{ 255.f };
	constexpr float deadZone{ 0.10f * maxValue };
	constexpr float fit{ maxValue - deadZone };

	float value{ m_pImpl->GetValue(binding.first) };

	if (value < deadZone)
	{
		return;
	}

	binding.second->SetValue((value - deadZone) / fit);
	binding.second->Execute();
}

void dae::Controller::UpdateBinding(const std::pair<const ControllerThumbsticks, std::unique_ptr<AxisValueCommand>>& binding) const
{
	constexpr float maxValue{ 32768 };
	constexpr float deadZone{ 0.15f * maxValue };
	constexpr float fit{ maxValue - deadZone };

	glm::vec2 value{ m_pImpl->GetValue(binding.first) };

	//Check deadZones
	if (abs(value.x) < deadZone && abs(value.y) < deadZone)
	{
		return;
	}

	if (abs(value.x) < deadZone)
	{
		value.x = 0.f;
	}
	else if (abs(value.y) < deadZone)
	{
		value.y = 0.f;
	}

	if (m_InvertY)
	{
		value.y *= -1;
	}

	binding.second->SetValue((value - deadZone) / fit);
	binding.second->Execute();
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

void dae::Controller::MapCommandToTrigger(ControllerTriggers trigger, std::unique_ptr<SingleValueCommand>&& pCommand)
{
	m_pTriggerCommands.emplace(trigger, std::move(pCommand));
}

void dae::Controller::MapCommandToThumbstick(ControllerThumbsticks thumbstick, std::unique_ptr<AxisValueCommand>&& pCommand)
{
	m_pThumbstickCommands.emplace(thumbstick, std::move(pCommand));
}

