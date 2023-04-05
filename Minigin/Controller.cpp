#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#include "Controller.h"

#include <unordered_map>

#include "Command.h"
#include "Value1DCommand.h"
#include "Value2DCommand.h"

#include <glm/glm.hpp>
#include <utility>

using namespace dae;

class Controller::ControllerImpl final
{
public:
	ControllerImpl(int controllerIndex, bool invertY)
		:m_ControllerIndex(controllerIndex), m_InvertY(invertY)
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

		UpdateButtonCommands();
		UpdateValueCommands();
	}

	void UpdateButtonCommands()
	{
		//Down
		for (const auto& binding : m_pButtonDownCommands)
		{
			if (IsDownThisFrame(binding.first))
			{
				binding.second->Execute();
			}
		}

		//Up
		for (const auto& binding : m_pButtonUpCommands)
		{
			if (IsUpThisFrame(binding.first))
			{
				binding.second->Execute();
			}
		}

		//Pressed
		for (const auto& binding : m_pButtonPressedCommands)
		{
			if (IsPressed(binding.first))
			{
				binding.second->Execute();
			}
		}
	}

	void UpdateValueCommands()
	{
		//Triggers
		for (const auto& binding : m_pTriggerCommands)
		{
			float value{ GetValue(binding.first) };

			if (abs(value) > 0.f)
			{
				binding.second->SetValue(value);
				binding.second->Execute();
			}
		}

		//Thumbsticks
		for (const auto& binding : m_pThumbstickCommands)
		{
			glm::vec2 value{ GetValue(binding.first) };

			if (glm::abs(value.x) > 0.f || glm::abs(value.y) > 0.f)
			{
				binding.second->SetValue(value);
				binding.second->Execute();
			}
		}
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
		glm::vec2 value{};

		if (thumbstick == ControllerThumbsticks::LeftThumbstick)
		{
			value = glm::vec2{ m_CurrentState.Gamepad.sThumbLX, m_CurrentState.Gamepad.sThumbLY };
		}
		else
		{
			value = glm::vec2{ m_CurrentState.Gamepad.sThumbRX,m_CurrentState.Gamepad.sThumbRY };
		}

		constexpr float maxValue{ 32768.f };
		constexpr float deadZone{ 0.15f * maxValue };
		constexpr float fit{ maxValue - deadZone };

		//Check deadZones
		if (glm::abs(value.x) < deadZone && glm::abs(value.y) < deadZone)
		{
			return glm::vec2{ 0.f,0.f };
		}

		if (glm::abs(value.x) < deadZone)
		{
			value.x = 0.f;
		}
		else if (glm::abs(value.y) < deadZone)
		{
			value.y = 0.f;
		}

		if (m_InvertY)
		{
			value.y *= -1;
		}

		return (value - deadZone) / fit;
	}

	float GetValue(ControllerTriggers trigger) const
	{
		float value{};

		if (trigger == ControllerTriggers::LeftTrigger)
		{
			value = m_CurrentState.Gamepad.bLeftTrigger;
		}
		else
		{
			value = m_CurrentState.Gamepad.bRightTrigger;
		}

		constexpr float maxValue{ 255.f };
		constexpr float deadZone{ 0.10f * maxValue };
		constexpr float fit{ maxValue - deadZone };

		if (value < deadZone)
		{
			return 0.f;
		}

		return (value - deadZone) / fit;
	}

	void MapCommandToButton(unsigned int button, std::unique_ptr<Command>&& pCommand, ButtonState state)
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

	void MapCommandToTrigger(ControllerTriggers trigger, std::unique_ptr<Value1DCommand>&& pCommand)
	{
		m_pTriggerCommands.emplace(trigger, std::move(pCommand));
	}

	void MapCommandToThumbstick(ControllerThumbsticks thumbstick, std::unique_ptr<Value2DCommand>&& pCommand)
	{
		m_pThumbstickCommands.emplace(thumbstick, std::move(pCommand));
	}

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	const int m_ControllerIndex;
	const bool m_InvertY{ true };

	//https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
	std::unordered_map<unsigned int, std::unique_ptr<Command>, std::hash<unsigned int >> m_pButtonUpCommands;
	std::unordered_map<unsigned int, std::unique_ptr<Command>, std::hash<unsigned int >> m_pButtonDownCommands;
	std::unordered_map<unsigned int, std::unique_ptr<Command>, std::hash<unsigned int >> m_pButtonPressedCommands;

	std::unordered_map<ControllerTriggers, std::unique_ptr<Value1DCommand>, std::hash<ControllerTriggers>> m_pTriggerCommands;
	std::unordered_map<ControllerThumbsticks, std::unique_ptr<Value2DCommand>, std::hash<ControllerThumbsticks>> m_pThumbstickCommands;
};

Controller::Controller(int controllerIndex, bool invertY)
	:m_pImpl(new ControllerImpl{ controllerIndex, invertY })
{
}

dae::Controller::~Controller()
{
	delete m_pImpl;
}

void Controller::Update()
{
	m_pImpl->Update();
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
	m_pImpl->MapCommandToButton(static_cast<unsigned int>(button), std::move(pCommand), state);
}

void dae::Controller::MapCommandToTrigger(ControllerTriggers trigger, std::unique_ptr<Value1DCommand>&& pCommand)
{
	m_pImpl->MapCommandToTrigger(trigger, std::move(pCommand));
}

void dae::Controller::MapCommandToThumbstick(ControllerThumbsticks thumbstick, std::unique_ptr<Value2DCommand>&& pCommand)
{
	m_pImpl->MapCommandToThumbstick(thumbstick, std::move(pCommand));
}

